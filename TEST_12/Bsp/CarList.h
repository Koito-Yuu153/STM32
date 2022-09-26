#ifndef	__CARLIST_H__
#define	__CARLIST_H__

#include		"main.h"

#define	MAX_SIZE		8

#define	OK	200
#define	ERROR	127


#define	CAR_IN		100
#define	CAR_OUT		101


typedef	struct		
{
	char	Type[5]; //停车类型
	char	CarNum[5];//车牌号
	
	uint8_t	Year; //年
	uint8_t	Month;//月
	uint8_t	Date;//日
	
	uint8_t	Hours; //时
	uint8_t	Minutes;//分
	uint8_t	Seconds;//秒
	
}CarMessageTypedef;

typedef	struct
{
	CarMessageTypedef	CarList[MAX_SIZE];
	uint8_t	Length;
	uint8_t	CNBR_Count;
	uint8_t	VNBR_Count;
}CarListTypedef;

typedef	struct
{
	float Price;
	uint8_t ParkHours;
}ParkMessageTypedef;

//uint8_t	Car_List_Init(CarListTypedef	* car_list);
uint8_t	Car_List_Add(CarListTypedef	* car_list,CarMessageTypedef*	Car);
uint8_t	Car_List_Delete(CarListTypedef* car_list,CarMessageTypedef*	car);
uint8_t	Car_List_Search(CarListTypedef* car_list,CarMessageTypedef*	car);

#endif

