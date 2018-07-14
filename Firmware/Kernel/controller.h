/******
	************************************************************************
	******
	** @file		: controller.cpp/h
	** @brief		: 伺服控制程序
	************************************************************************
	******
	** @versions	: 1.0.0
	** @time		: 2018/5/20
	************************************************************************
	******
	** @authors		: unli (China WuHu)
	************************************************************************
	******
	** @explain		:
	** 	1.控制环路与编码器数据矫正
	** 	2.Control loop and encoder data correction
	************************************************************************
	************************************************************************
******/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

//引用的实例
#include "stm32f1xx_hal.h"

/***************************************************************************/
/*********************************** PID ***********************************/
/***************************************************************************/
//PID_Typedef
typedef struct{
	//参数量
	float Kp, Ki, Kd, Kq;			//PIDQ
	//过程量
	float OUTp, OUTi, OUTd, OUTq;	//OutPIDQ
	float error, error_l;			//error
	bool  overshoot;				//超调标志
}PID_Typedef;

//PID清空函数
void PID_Empty(PID_Typedef *st);


/***************************************************************************/
/****************************** Controller *********************************/
/***************************************************************************/
#define Controller_Freq		(10000.0f)			//控制频率

//控制模式
enum{
	Control_Mode_PosiVelo = 0,	//二阶角位置模式
	Control_Mode_Position,		//角位置模式
	Control_Mode_Velocity,		//角速度模式
	Control_Mode_Torque,		//力矩模式
	Control_Mode_Disable,		//失能模式
};

//输入模式
enum{
	Import_Mode_Pulse = 0,		//脉冲输入模式
	Import_Mode_Digital,		//数字输入模式
};

//
enum{
	Drive_Module_Standard = 0,	//标准版驱动模组(A4950)
	Drive_Module_Power,			//动力版驱动模组(MOS)
};

//
//Controller Class
//
class Controller
{
public:
	//测试数据
	float test_data[20];
	//控制参数
	uint32_t spr;			//每转步数			(自动寻找)
	float map;				//角度->数组编号	(spr运算得)
	float esa;				//每步角度			(spr运算得)
	uint32_t sdb;			//每步细分步数		(写入值，自定义设置)
	bool dir_pm;			//方向				(写入值，自定义设置)
	float t_pm;				//力矩->电流		(写入值，根据电机使用设置)
	uint32_t drive_module;	//驱动模组			(写入值，根据使用的驱动模组设置)
	//控制实体
	PID_Typedef		pgv_pid;	//位置速度PID
	PID_Typedef		p_pid;		//位置单环PID
	PID_Typedef		v_pid;		//速度单环PID
	//滤波数据
	float gpvLPF, gpvLPFa, gpvLPFb;	//位置速度滤波数据
	float pLPF, pLPFa, pLPFb;		//位置滤波数据
	float vLPF, vLPFa, vLPFb;		//速度滤波数据
	float iLPF, iLPFa, iLPFb;		//电流输出滤波数据
	//过程量
	uint32_t control_mode;	//控制模式
	uint32_t import_mode;	//输入模式
	int64_t step_count;		//记录输入脉冲数
	int32_t circle_count;	//旋转圈数
	bool up_st;				//标准化指令
	//输入输出
	uint16_t enread;										//编码器数据
	float last_sensor_p;									//记录传感器原始
	float last_read_p;										//记录读取原始
	float last_filter_gpv, last_filter_p, last_filter_v;	//记录滤波数据
	float last_goal_p;										//记录目标数据
	float sensor_p;											//传感器原始
	float read_p;											//读取原始
	float filter_gpv, filter_p, filter_v, filter_a;			//滤波数据
	float goal_p, goal_v, goal_t;							//目标数据
	float out_p,  out_i;									//输出数据
	// float filter_p, filter_pv, filter_v, filter_a, filter_t, filter_i;	//滤波数据
	// float goal_p,   goal_pv,   goal_v,   goal_a,   goal_t,   goal_i;		//目标数据
	// float out_p,    out_pv,    out_v,    out_a,    out_t,    out_i;		//输出数据
	
	/********** 公共函数 **********/
	Controller();
	//配置
	void up_spr(uint32_t _spr);						//更新spr
	void Start(void);								//控制器启动
	void Data_conv_PtD(void);						//数据变换PtD
	void Data_conv_DtP(void);						//数据变换DtP
	//引用
	void Debug_new_goal(void);						//给调试位置速度
	void Callback(void);							//控制回调函数
	bool Standardizing(void);						//标准化
	//输出
	void PhaseOut_Stop(void);						//相位输出停止
	void PhaseOut_A(float p, float i); 				//相位输出
	void PhaseOut_S(uint32_t p, float i);			//相位输出
private:
	
};

#endif	//CONTROLLER_H

