with system ; use system
with ada.real_Time; use ada.real_time;

Package Glucose_sensor is
type glucose_reading is new range 1..20;
period :Time_span := Milliseconds(100);
Thread_Priority : ConstantPriority :=8;
end Glucose_sesnor

Package voltage_sensor is
type Voltage_reading is new range 1..20;
period :Time_span := Milliseconds(100);
Thread_Priority : ConstantPriority :=8;
end Voltage_sesnor;

with System; use System;
With Ada.Real_Time; use Ada.Text_IO;
with Glucose_sensor_Rtatt ; use co_sensor_Rtatt;
with Voltage_sensor_Rtatt; use Voltage_sensor_Rtatt;
with Process2; use Process2;
with glucosecsr; use glucosecsr;
with Voltagecsr; use Voltagecsr;
with Device_Register_Type; use Device_Register_Types;

package body Glucose_sensor is
Glucose_present : Glucose_reading;

Procedure initialise is
begin
Glucosecsr.Device(Enable);
voltagecsr.Device(Enable);
Glucosecsr.Operation(Set);
Voltagecsr.Operation(Set);
End initialise;

Procedure Process2 is
begin

if not Glucosecsr.Done  then
     Operator_console.Alarm(Glucose_Device_Error);
else
    Glucose_present:= GlucoseDbr;
if not VoltageCsr.done then
     Operator_console.Alarm(Glucose_Device_Error);
else
    voltage_present:= voltageDbr;
    ConversionValueOFVoltageTOGlucose:= VoltageSensor;
    Glucose_Sensor := ConversionValueOFVoltageTOGlucose;

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



