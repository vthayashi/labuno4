# labuno4
Open Source Oscilloscope built with Flask, pyserial, HTML, javascript and Arduino Uno.

<img src="https://github.com/vthayashi/labuno4/blob/master/imgREADME.png" alt="LabUno Desktop interface" width="800"/>

## Built With

* [Timer One](https://github.com/PaulStoffregen/TimerOne) - Arduino Library
* [Flask](https://flask.palletsprojects.com/en/1.1.x/) - Python Library
* [Pyserial](https://github.com/pyserial/pyserial) - Python Library

## Contributing

Please contact the [author](https://www.linkedin.com/in/victor-hayashi-885083131/) for contributing with this work.

## Authors

* **Fabio Hayashi** - *Initial work* - [fabio hayashi](https://www.linkedin.com/in/fabio-hayashi-bab61914/)

* **Victor Hayashi** - *Reviewer* - [vthayashi](https://github.com/vthayashi)

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* **Review: Haroldo Amaral** - [agaelema](https://github.com/agaelema)

## FAQ

* Change USB port to corresponding port where your Arduino device is connected, on app.py:

ser = serial.Serial(**'COM7'**, baudrate=500000, timeout=10)

* If you are on Linux (Ubuntu 16.04 LTS), comment the line below on app.py:

ser.set_buffer_size(rx_size = 25600, tx_size = 12800)
