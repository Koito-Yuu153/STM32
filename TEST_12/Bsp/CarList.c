#include	 "CarList.h"
#include		<string.h>
//初始化顺序表
//uint8_t	Car_List_Init(CarListTypedef	* car_list)
//{
//	uint8_t i=0,j=0;
//	for(j=0;i<MAX_SIZE;i++)
//	{
//		for(j=0;j<5;j++)
//		{
//			car_list->CarList[i].Type[j]=' ';
//			car_list->CarList[i].CarNum[j]=' ';
//		}
//		for(j=0;j<3;j++)
//		{
//			car_list->CarList[i].Year[j]=' ';
//			car_list->CarList[i].Month[j]=' ';
//			car_list->CarList[i].Date[j]=' ';
//			
//			car_list->CarList[i].Hours[j]=' ';
//			car_list->CarList[i].Minutes[j]=' ';
//			car_list->CarList[i].Seconds[j]=' ';
//		}
//		car_list->Length=0;
//	}
//	return OK;
//}

uint8_t	Car_List_Add(CarListTypedef	* car_list,CarMessageTypedef*	car)
{
	if((car_list->Length)<=MAX_SIZE)	//数组没满,插入新元素
	{
		car_list->CarList[car_list->Length]=*car;
		car_list->Length++;
		return OK;
	}
	return  ERROR;
}

uint8_t	Car_List_Delete(CarListTypedef* car_list,CarMessageTypedef*	car)
{
	uint8_t	i=0,j=0;
	if(car_list->Length>0)
	{
		j=Car_List_Search(car_list,car);
		
		if(car_list->VNBR_Count>=1&&memcmp(car->Type,"VNBR",4)==0)
			car_list->VNBR_Count--;
		else if(car_list->CNBR_Count>=1&&memcmp(car->Type,"CNBR",4)==0)
			car_list->CNBR_Count--;
		
		if(j<car_list->Length-1)
		{
			for(i=j;(i<car_list->Length-1);i++)
			{
				car_list->CarList[i]=car_list->CarList[i+1];
			}
			car_list->Length--;
			return OK;
		}
		else if(j==car_list->Length-1)
		{
			car_list->Length--;
			return OK;
		}
	}

	return	ERROR;
}

/**
  * @brief  
  * @param  	car_list:车辆信息列表，car:车辆信息
	* @retval 	i:返回匹配成员的序列号
  */
uint8_t	Car_List_Search(CarListTypedef* car_list,CarMessageTypedef* car)
{
	uint8_t i=0;
	
	if(car_list->Length!=0)
	{
		for(i=0;i<car_list->Length;i++)
		{
			if((memcmp(car_list->CarList[i].CarNum,car->CarNum,4)==0))
			{
				//进入这里说明已经存在编号相同的车辆
				if(memcmp(car_list->CarList[i].Type,car->Type,4)!=0)//停车类型不同则说明指令逻辑错误:停车类型错误
				{return ERROR;}
				
				//到这里说明编号类型都相同，检查时间逻辑，传进来的时间要比表中的时间大
				if(car_list->CarList[i].Year<=car->Year && car_list->CarList[i].Month<=car->Month && car_list->CarList[i].Date<=car->Date &&
					 car_list->CarList[i].Hours<=car->Hours && car_list->CarList[i].Minutes<=car->Minutes&&car_list->CarList[i].Seconds<=car->Seconds)
				{
					return i;
				}
				else
				{return ERROR;} //逻辑错误:时间不对
			}
		}
	}
	//执行到这里说明这辆车是入库的(车库里没有)
	if(memcmp(car->Type,"VNBR",4)==0)
		car_list->VNBR_Count++;
	else
		car_list->CNBR_Count++;
	return	CAR_IN;
}
