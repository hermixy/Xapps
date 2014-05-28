#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
int main(int argc, char *arg[])
{
    int s, nbytes, i;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter[1];

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);  //�����׽���
    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);     //ָ��can0�豸
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));   //���׽�����can0��
  //������չ���ֻ���ձ�ʾ������0x181�ı���
    rfilter[0].can_id   = 0x181;
    rfilter[0].can_mask = CAN_SFF_MASK;
//���ù��˹���
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    while(1) {
       nbytes = read(s, &frame, sizeof(frame));   //���ձ���
   //��ʾ����
       if (nbytes > 0) {
           printf("ID=0x%X DLC=%d data[0]=0x%X\n", frame.can_id,
           frame.can_dlc, frame.data[0]);
       }

       frame.can_id = 0x123;
       frame.can_dlc = 8;
       for(i=0; i<8; i++){
    	   frame.data[i] ^= 0xFF;
       }

       nbytes = write(s, &frame, sizeof(frame));  //����frame[0]
       if (nbytes != sizeof(frame)) {
    	   printf("Send Error frame[0]\n!");
    	   break;        //���ʹ����˳�
       }
       printf("Send Back data[0]=0x%X\n",frame.data[0]);
    }
    close(s);
    return 0;
}
