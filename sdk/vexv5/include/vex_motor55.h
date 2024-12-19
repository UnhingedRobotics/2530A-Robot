/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Innovation First 2017-2024, All rights reserved.          */
/*                                                                            */
/*    Module:     vex_motor55.h                                               */
/*    Author:     James Pearman                                               */
/*    Created:    22 May 2024                                                 */
/*                                                                            */
/*    Revisions:                                                              */
/*                V1.00     TBD - Initial release                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef   VEX_MOTOR55_CLASS_H
#define   VEX_MOTOR55_CLASS_H

/*-----------------------------------------------------------------------------*/
/** @file    vex_motor55.h
  * @brief   Motor device class header for MC55 controller board.
*//*---------------------------------------------------------------------------*/

namespace vex {
  /** 
    * @brief Use the motor55 class to control MC55 devices.
  */
  class motor55 : public device {
    public:
     /** 
      * @brief Creates a new motor55 object on the port specified.
      * @param index The port index for this motor. The index is zero-based.
      */
      motor55( int32_t index );  
      ~motor55();
    
      bool            installed();
      int32_t         value();

      /** 
       * @brief Creates a new motor55 object on the port specified and sets the reversed flag.
       * @param index The port index for this motor. The index is zero-based.
       * @param reverse Sets the reverse flag for the new motor object.
       */
      motor55( int32_t index, bool reverse );

      /** 
       * @brief Creates a new motor55 object on the port specified and sets the reversed flag.
       * @param index The port index for this motor. The index is zero-based.
       * @param maxv Sets the maximum drive voltage for the motor.
       * @param reverse Sets the reverse flag for the new motor object.
       */
      motor55( int32_t index, double maxv, bool reverse = false );

      /** 
       * @brief Sets the motor mode to "reverse", which will make motor commands spin the motor in the opposite direction.
       * @param value If set to true, motor commands spin the motor in the opposite direction.
       */
      void            setReversed( bool value );

      //Actions
      /** 
       * @brief Sets the velocity of the motor based on the parameters set in the command. This command will not run the motor.  Any subsequent call that does not contain a specified motor velocity will use this value.
       * @param velocity Sets the amount of velocity.
       * @param units Velocities can only be expressed in percentage for a MC55
       */
      void            setVelocity( double velocity, percentUnits units = percent );

      /** 
       * @brief Sets the stopping mode of the motor by passing a brake mode as a parameter.
       * @param mode The stopping mode can be set to coast or brake, hold is not supported on the MC55.  
       */
      void            setStopping( brakeType mode );

      /** 
       * @brief Turns the motor on, and spins it in the specified direction.
       * @param dir The direction to spin the motor.
       */
      void            spin( directionType dir );

      /**
       * @brief Turns on the motor and spins it in a specified direction and a specified voltage.
       * @param dir The direction to spin the motor. 
       * @param voltage Sets the amount of volts.
       * @param units The measurement unit for the voltage value. 
       */
      void            spin( directionType dir, double voltage, voltageUnits units );

      /** 
       * @brief Stops the motor using the default brake mode.
       */
      void            stop( void );
      
      /** 
       * @brief Sets the max torque of the motor.
       * @param value Sets the amount of torque.
       * @param units The unit for the torque value, only amp is supported for an MC55.
       */
      void            setMaxTorque( double value, currentUnits units = currentUnits::amp );
      
      // sensing

      /** 
       * @brief Gets the electrical current of the motor.
       * @return Returns a double that represents the electrical current of the motor in the units defined in the parameter.
       * @param units The measurement unit for the current.
       */
      double          current( currentUnits units = currentUnits::amp );

      /** 
       * @brief Gets the electrical current of the motor in percentage of maximum.
       * @return Returns a double that represents the electrical current of the motor as percentage of max current.
       * @param units The measurement unit for the current.
       */
      double          current( percentUnits units );

      /** 
       * @brief Gets the electrical voltage of the motor.
       * @return Returns a double that represents the electrical voltage of the motor in the units defined in the parameter.
       * @param units The measurement unit for the voltage.
       */
      double          voltage( voltageUnits units = voltageUnits::volt );

      /** 
       * @brief Gets the temperature  of the motor.
       * @return Returns the temperature of the motor in the units defined in the parameter.
       * @param units The measurement unit for the temperature.
       */
      double          temperature( percentUnits units = percentUnits::pct );
      
      /** 
       * @brief Gets the temperature  of the motor.
       * @return Returns the temperature of the motor in the units defined in the parameter.
       * @param units The measurement unit for the temperature.
       */
      double          temperature( temperatureUnits units );

      /** 
       * @brief Get the type of the motor, 11W, 5.5W or MC55.
       * @return Returns the type of the motor, 0 is 11W, 1 is 5.5W, 2 is MC55
       */
      int32_t         getMotorType();            

    protected:
      double          getMaxVoltage(voltageUnits units = voltageUnits::volt);
    
    private:
      brakeType       _mode;
      brakeType       _brakeMode;
      bool            _spinMode;
      double          _voltage;            
      int32_t         _max_voltage_mv;            
  };
};

#endif // VEX_MOTOR55_CLASS_H