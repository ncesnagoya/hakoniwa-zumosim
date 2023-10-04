void doOperation(void)
{
    if (led_status) {
        led_status = false;
        led.off();
        motors.setSpeeds(0, 5);
    }
    else {
        led_status = true;
        led.on();
        motors.setSpeeds(1, 1);
    }
    rf_sensor.update();
    int i;
    for (i = 0; i < ZUMOSIM_REFLECT_SENSOR_NUM; i++) {
        std::cout << "[" << i << "] = " << rf_sensor.value(i) << std::endl;
    }
    std::cout << "compass: " << compass.averageHeading() << std::endl;
    zumo_delay(5000);
}
