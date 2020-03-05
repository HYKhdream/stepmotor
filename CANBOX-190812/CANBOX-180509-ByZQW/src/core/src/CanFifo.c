#include "canfifo.h"

typedef struct
{
	unsigned long fifo_total;
	int fifo_size;
	int fifo_wptr;
	int fifo_rptr;
	CAN_MSG_DEF *fifo_buf;
} CAN_FIFO_DEF;

static int can_recv_disable = 1;
#define RECV_BUFF_MAX	50
CAN_MSG_DEF RecvBuf[RECV_BUFF_MAX];
CAN_FIFO_DEF RecvFifo;
unsigned long recv_filter_count;

#define SHOW_BUFF_MAX	1000
CAN_MSG_DEF ShowBuf[SHOW_BUFF_MAX];
CAN_FIFO_DEF ShowFifo;



 
#define FILTER_ID_MAX	8
////static int Filter_ID[FILTER_ID_MAX];
////static int Filter_ID_enable;

#define FILTER_DATA_GRP	3
////static int Filter_DATA[FILTER_DATA_GRP][8];
////static int Filter_DATA_enable;

#define FILTER_PARA_MAGIC	(((long)'F' << 24) + ((long)'T' << 16) | ((long)'P' << 8) | ((long)'D'))
#define FILTER_PARA_VERSION	0x00000001
typedef struct {
	unsigned long Filter_magic;
	unsigned long Filter_version;
	int Filter_ID[FILTER_ID_MAX];
	int Filter_ID_enable;
	int Filter_DATA[FILTER_DATA_GRP][8];
	int Filter_DATA_enable;
} FILTER_DEF;

FILTER_DEF FilterPara;

unsigned int can_bps_val = 0;
#define COUNTOF(a)	(sizeof(a) / sizeof(a[0]))
#define CAN_BPS_ADDR		sizeof(FILTER_DEF)

unsigned long arch_get_systicks(void);

void arch_eeprom_read(void *pDat, int addr, int count);
void arch_eeprom_write(void *pDat, int addr, int count);


int can_bps_save(unsigned int val)
{
	can_bps_val = val;
	
	if (can_bps_val > 1000 || can_bps_val <= 0)
	{
		can_bps_val = 1000;
	}
	arch_eeprom_write((void *)&can_bps_val, CAN_BPS_ADDR, 4);
	
	return 0;
}

int can_bps_load(void)
{
	arch_eeprom_read((void *)&can_bps_val, CAN_BPS_ADDR, 4);
	if (can_bps_val > 1000 || can_bps_val <= 0)
	{
		can_bps_val = 1000;
	}
	return can_bps_val;
}

int can_bps_get(void)
{
	return can_bps_load();
}

int can_para_load(FILTER_DEF *pPara)
{
	arch_eeprom_read(pPara, 0, 8);
	if(pPara->Filter_magic != FILTER_PARA_MAGIC)
		return -1;
	arch_eeprom_read(pPara, 0, sizeof(FILTER_DEF));

	return 0;
}

void can_fifo_para_load()
{
	can_para_load(&FilterPara);
}

void can_para_save()
{
	FILTER_DEF Para;

	can_para_load(&Para);

	if(memcmp(&Para, &FilterPara, sizeof(FILTER_DEF)) == 0)
		return ;

	FilterPara.Filter_magic = FILTER_PARA_MAGIC;
	FilterPara.Filter_version = FILTER_PARA_VERSION;
	arch_eeprom_write(&FilterPara, 0, sizeof(FilterPara));
}

void can_fifo_init()
{
	RecvFifo.fifo_size = RECV_BUFF_MAX;
	RecvFifo.fifo_wptr = 0;
	RecvFifo.fifo_total = 0;
	RecvFifo.fifo_rptr = 0;
	RecvFifo.fifo_buf = RecvBuf;

	ShowFifo.fifo_size = SHOW_BUFF_MAX;
	ShowFifo.fifo_wptr = 0;
	ShowFifo.fifo_total = 0;
	ShowFifo.fifo_rptr = 0;
	ShowFifo.fifo_buf = ShowBuf;
}

static int CF__can_fifo_count(CAN_FIFO_DEF *fifo)
{
	int count = fifo->fifo_size + fifo->fifo_wptr - fifo->fifo_rptr;
	if(count >= fifo->fifo_size)
		count -= fifo->fifo_size;

	return count;
}

static int CF__can_fifo_is_empty(CAN_FIFO_DEF *fifo)
{
	return (CF__can_fifo_count(fifo) <= 0);
}

static int CF__can_fifo_is_full(CAN_FIFO_DEF *fifo)
{
	return (CF__can_fifo_count(fifo) >= fifo->fifo_size - 1);
}

static int CF__can_fifo_drop(CAN_FIFO_DEF *fifo, int mode)
{
	int rptr;
	if(CF__can_fifo_is_empty(fifo))
		return 0;

	if(mode) {
		fifo->fifo_rptr = fifo->fifo_wptr;
		return 1;
	}

	rptr = fifo->fifo_rptr;

	rptr ++;
	if(rptr >= fifo->fifo_size)
		rptr -= fifo->fifo_size;
	fifo->fifo_rptr = rptr;

	return 1;
}

static CAN_MSG_DEF *CF__can_fifo_loc(CAN_FIFO_DEF *fifo, int loc)
{
	int rptr;
	if(CF__can_fifo_is_empty(fifo))
		return NULL;

	rptr = fifo->fifo_rptr + loc;
	if(rptr >= fifo->fifo_size)
		rptr -= fifo->fifo_size;

	return &fifo->fifo_buf[rptr];
}
#if 1
static int CF__can_fifo_pop(CAN_FIFO_DEF *fifo, unsigned long *ulMsgID, int *MsgLen, unsigned char *Msg, int mode)
{
	int rptr;
	if(CF__can_fifo_is_empty(fifo))
		return 0;

	rptr = fifo->fifo_rptr;

	if(ulMsgID)
		*ulMsgID = fifo->fifo_buf[rptr].msg_id;
	if(MsgLen)
		*MsgLen = fifo->fifo_buf[rptr].msg_len;
	if(Msg)
		memcpy(Msg, fifo->fifo_buf[rptr].msg_dat, fifo->fifo_buf[rptr].msg_len);

	rptr ++;
	if(rptr >= fifo->fifo_size)
		rptr -= fifo->fifo_size;
	fifo->fifo_rptr = rptr;

	return 1;
}
#endif
/*


******/
unsigned long CWJ_RECV_ID;
int CWJ_RECV_LEN;
unsigned char CWJ_RECV_DATA[8];
void can_data_read(unsigned long ulMsgID, int MsgLen, unsigned char *Msg)
{
	int i;
	CWJ_RECV_ID=ulMsgID;
	CWJ_RECV_LEN=MsgLen;
	
	memcpy(CWJ_RECV_DATA, Msg, MsgLen);
	
}

static int CF__can_fifo_push(CAN_FIFO_DEF *fifo, unsigned long ulMsgID, int MsgLen, unsigned char *Msg, int mode)
{
	int wptr;
	if(CF__can_fifo_is_full(fifo)) {
		if(mode == 0) {
			return 0;
		}

		// drop the old msg
		CF__can_fifo_drop(fifo, 0x0);
	}

	wptr = fifo->fifo_wptr;

	fifo->fifo_buf[wptr].msg_id = ulMsgID;
	fifo->fifo_buf[wptr].msg_len = MsgLen;
	fifo->fifo_buf[wptr].msg_time = arch_get_systicks();
	memcpy(fifo->fifo_buf[wptr].msg_dat, Msg, MsgLen);

	wptr ++;
	if(wptr >= fifo->fifo_size)
		wptr -= fifo->fifo_size;
	fifo->fifo_wptr = wptr;
	fifo->fifo_total ++;

	return 1;
}

static int CF__can_fifo_push_msg(CAN_FIFO_DEF *fifo, CAN_MSG_DEF *Msg, int mode)
{
	int wptr;
	if(CF__can_fifo_is_full(fifo)) {
		if(mode == 0) {
			return 0;
		}

		// drop the old msg
		CF__can_fifo_drop(fifo, 0x0);
	}

	wptr = fifo->fifo_wptr;

	memcpy(&fifo->fifo_buf[wptr], Msg, sizeof(CAN_MSG_DEF));

	wptr ++;
	if(wptr >= fifo->fifo_size)
		wptr -= fifo->fifo_size;
	fifo->fifo_wptr = wptr;
	fifo->fifo_total ++;

	return 1;
}

int can_fifo_push(unsigned long ulMsgID, int MsgLen, unsigned char *Msg)
{
	if(can_recv_disable)
		return 0;

	return CF__can_fifo_push(&RecvFifo, ulMsgID, MsgLen, Msg, 0x1);
}

int can_filter_id_get(int idx)
{
	if(idx >= FILTER_ID_MAX)
		return 0;
	return FilterPara.Filter_ID[idx];
}

int can_filter_id_set(int idx, int ID)
{
	if(idx >= FILTER_ID_MAX)
		return 0;
	return FilterPara.Filter_ID[idx] = ID;
}

int can_filter_id_save()
{
	int i, j;

	for(i = 0; i < FILTER_ID_MAX - 1; i++) {
		for(j = i + 1; j < FILTER_ID_MAX; j++) {
			if((FilterPara.Filter_ID[j] != 0) &&
			   ((FilterPara.Filter_ID[i] <= FilterPara.Filter_ID[j]) ||
			    (FilterPara.Filter_ID[i] == 0))) {
				int ID;
				if(FilterPara.Filter_ID[i] == FilterPara.Filter_ID[j]) {
					FilterPara.Filter_ID[j] = 0;
					continue;
				}
				ID = FilterPara.Filter_ID[i];
				FilterPara.Filter_ID[i] = FilterPara.Filter_ID[j];
				FilterPara.Filter_ID[j] = ID;
			}
		}
	}
	FilterPara.Filter_ID_enable = 0;
	for(i = 0; i < FILTER_ID_MAX; i++) {
		if(FilterPara.Filter_ID[i] != 0) {
			FilterPara.Filter_ID_enable = 1;
			break;
		}
	}

	return FilterPara.Filter_ID_enable;
}

int can_filter_id_check(unsigned long ulMsgID)
{
	int i;
	if(FilterPara.Filter_ID_enable == 0)
		return 1;

	for(i = 0; i < FILTER_ID_MAX; i++) {
		if(FilterPara.Filter_ID[i] == 0) {
			continue;
		}
		if(FilterPara.Filter_ID[i] == ulMsgID) {
			return 1;
		}
	}

	return 0;
}

int can_filter_data_get(int group, int idx)
{
	if(idx >= 8)
		return 0;
	return FilterPara.Filter_DATA[group][idx];
}

int can_filter_data_set(int group, int idx, int Dat)
{
	if(idx >= 8)
		return 0;
	return FilterPara.Filter_DATA[group][idx] = Dat;
}

int can_filter_data_save()
{
	int i;
	int j;

	FilterPara.Filter_DATA_enable = 0;
	for(j = 0; j < FILTER_DATA_GRP; j++) {
	for(i = 0; i < 8; i++) {
		if(FilterPara.Filter_DATA[j][i] != 0) {
			FilterPara.Filter_DATA_enable |= 1L << j;
		}
	}
	}

	return FilterPara.Filter_DATA_enable;
}

int can_filter_data_check(unsigned char *dat)
{
	int i;
	int j;

	if(FilterPara.Filter_DATA_enable == 0) {
		return 1;
	}

	for(j = 0; j < FILTER_DATA_GRP; j++) {
		if(!(FilterPara.Filter_DATA_enable & (1L << j))) {
			continue;
		}
		for(i = 0; i < 8; i++) {
			if(FilterPara.Filter_DATA[j][i] == 0)
				continue;
			if(dat[i] != FilterPara.Filter_DATA[j][i]) {
				goto next;
			}
		}
		return 1;
	next:	;
	}

	return 0;
}

int can_filter_get_enable()
{

	int ret = 0;
	if (FilterPara.Filter_ID_enable)
	{
		ret |= 0x1;
	}

	if (FilterPara.Filter_DATA_enable)
	{

		ret |= 0x2;
	}
	return ret;
}

void do_filter(void)
{
	CAN_MSG_DEF *Msg;

	Msg = CF__can_fifo_loc(&RecvFifo, 0);
	if(Msg == NULL) {
		return ;
	}
	if(Msg->msg_len <= 0) {
		goto add_it;
	}

	// Filter ID
	if(can_filter_id_check(Msg->msg_id) <= 0) {
		recv_filter_count++;
		goto done;
	}

	// Filter Data
	if(can_filter_data_check(Msg->msg_dat) <= 0) {
		recv_filter_count++;
		goto done;
	}

add_it:
	CF__can_fifo_push_msg(&ShowFifo, Msg, 0x1);
done:
	CF__can_fifo_drop(&RecvFifo, 0x0);
}

int CanRecv_FilterCount()
{
	return recv_filter_count;
}

void ShowFifo_push(unsigned long MsgID, int len, unsigned char *data)
{
	CAN_MSG_DEF Msg;

	Msg.msg_id = MsgID;
	Msg.msg_len = len;
	memcpy(Msg.msg_dat, data, len);
	Msg.msg_time = arch_get_systicks();

	CF__can_fifo_push_msg(&ShowFifo, &Msg, 0x1);
}

int CanRecv_Clear()
{
	RecvFifo.fifo_rptr = RecvFifo.fifo_wptr;
	RecvFifo.fifo_total = 0;
	ShowFifo.fifo_rptr = ShowFifo.fifo_wptr;
	ShowFifo.fifo_total = 0;
	recv_filter_count = 0;

	return 0;
}

int CanRecv_TopIdx()
{
	return ShowFifo.fifo_rptr;
}

int CanRecv_Count()
{
	return CF__can_fifo_count(&ShowFifo);
}

int CanRecv_Data(unsigned long *tMsgID,unsigned char *tMsg)
{
	if (tMsg == NULL)
		return -1;
	CF__can_fifo_pop(&ShowFifo, tMsgID, 0, tMsg, 0);
	return 0;
}

unsigned long CanRecv_Total()
{
	return ShowFifo.fifo_total;
}

int CanRecv_FramePrint_dat(int loc, char *buf)
{
	CAN_MSG_DEF *Msg;
	int i;

	Msg = CF__can_fifo_loc(&ShowFifo, loc);
	if(Msg == NULL) {
		return -1;
	}

//	sprintf(buf, "%03X:", Msg->msg_id);
	for(i = 0; i < 8; i++) {
		buf[i]=Msg->msg_dat[i];
//		if(i < Msg->msg_len)
//			sprintf(buf, "%s%02X ", buf, Msg->msg_dat[i]);
//		else
//			sprintf(buf, "%sXX ", buf);
	}
//	sprintf(buf, "%s%-6d", buf, Msg->msg_time);

	return 1;
}

int CanRecv_FramePrint_loc(int loc, char *buf)
{
	CAN_MSG_DEF *Msg;
	int i;

	Msg = CF__can_fifo_loc(&ShowFifo, loc);
	if(Msg == NULL) {
		return -1;
	}

	sprintf(buf, "%03X:", Msg->msg_id);
	for(i = 0; i < 8; i++) {
		if(i < Msg->msg_len)
			sprintf(buf, "%s%02X ", buf, Msg->msg_dat[i]);
		else
			sprintf(buf, "%sXX ", buf);
	}
	sprintf(buf, "%s%-6d", buf, Msg->msg_time);

	return 1;
}

int CanRecv_pick(int loc, CAN_MSG_DEF **Msg)
{
	if (Msg == NULL)
		return -1;

	*Msg = CF__can_fifo_loc(&ShowFifo, loc);
	if (Msg == NULL)
		return -1;
	return 0;
}



int CanRecv_status()
{
	return !can_recv_disable;
}

void canRecv_Toggle()
{
	can_recv_disable = !can_recv_disable;
}

void can_buf_init()
{
	can_fifo_init();

	can_recv_disable = 1;

	memset(&FilterPara, 0, sizeof(FilterPara));
}

int can_export_uart(int start, int end)
{
	CAN_MSG_DEF *Msg;
	char buf[64];
	int total;
	int ret;

	total = CanRecv_Count();
	if (end == 0)
	{
		end = total;
	}
	
	printf("\r\nTotal %d\r\n", total);
	if (start > total || end > total)
	{
		printf("Input num too larger\r\n");
		return -1;
	}
	
	for ( ; start < end; start++)
	{
		ret = CanRecv_FramePrint_loc(start, buf);
		if (ret < 0)
			return -1;

		printf("%4d : %s\r\n", start, buf);
	}

	return 1;
}


