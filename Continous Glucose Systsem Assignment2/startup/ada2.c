with system ; use system
with ada.real_Time; use ada.real_time;

Package insulin_motor is
type motor_reading is new range 1..20;
period :Time_span := Milliseconds(100);
Thread_Priority : ConstantPriority :=8;
end motor_sesnor


with System; use System;
With Ada.Real_Time; use Ada.Text_IO;
with motor_sensor_Rtatt ; use motor_sensor_Rtatt;
with motor; use motor;
with motorcsr; use mototrcsr;

with Device_Register_Type; use Device_Register_Types;

package body Insulin_motor is
motor_present : motor_reading;

Procedure initialise is
begin
motorcsr.Device(Enable);
motorcsr.Operation(Set);
End initialise;

Procedure Process6 is
begin

if not motorcsr.Done  then
     Operator_console.Alarm(motor_Device_Error);
else
    motor_present:= GlucoseDbr;
    procedure pump_insulin;
    procedure dont_pump_insulin;
    function Request_status return Operational_status
    Procedure: set_Pump(To:Pump_Status);
    private
    motor_status : Pump_status:= off;
    end Agent;

    procedure pump_insulin is
    begin
    agent.insulin_required;'
    end insulin_required;

    procedure dont_pump_insulin is
    begin
    Agent.insulin_Not_required;'
    end insulin_Not_required;

    procedure pump_insulin is
    begin
    if BloodSugar = High and Pressure = high or pressure = low;
    moter_status := pump_insulin;
    Motorcsr.operation(Clear);
    end if
    Motor_condition := on ;
    end Pump_insulin

     procedure pump_insulin is
    begin
    if BloodSugar = low and Pressure = high or pressure = low;
    moter_status := Dont_pump_insulin;
    Motorcsr.operation(Clear);
    end if
    Motor_condition := off ;
    end Pump_insulin



    begin
    T:= Clock +Period;
    initilise
    loop
    delay until(t)
    Period_code
    T:= T +Period;
    end Loop;
    end thread;
    end Glucose_Sensor and Voltage_sesnor;

