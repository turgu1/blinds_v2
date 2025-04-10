## BLINDS V2 433Mhz Controller

This is an example of a blinds 433Mhz based controller. It receives commands from the MQTT Broker and transmit control commands to blinds through a RF 433Mhz transmitter.

This version is now being built using the ESPHome open-source system. You must have installed it on your computer using the installation procedure available at the following [location](https://esphome.io/guides/installing_esphome.html).

The application requires the presence of a file named `secrets.yaml` located in the main application folder. To do so, please

1. Update the supplied `secrets.example.yaml` file with your network configuration parameters
2. Rename it to `secrets.yaml`

Use the following commands in a terminal window to compile and upload the application to the ESP32-based controller. After moving to the application folder, do the following:

``` sh
esphome compile main.yaml
esphome upload main.yaml
```

The controller expects receiving messages from the defined `mqtt_command_topic`. The message must be composed of the following three comma separated parameters on a single line:

1. The command. One of the following words (uppercase): `DOWN`, `UP`, `STOP`, `PAIR`, `LIMIT`, or `CHGDIR`.
2. The Blind's Device Identification Number
3. The channel numbers: 1 to 14 for a specific channel, 15 to send to all channels. Many channels can be identified, adding comma separated channel numbers after the first one.

Example messages:

- DOWN,65530,1
- UP,65530,1,3,5

Once received, the controller will compute the message checksum and send the coded 40 bits message through the connected 433Mhz transmitter. The message will be repeated 10 times in a row.

The controller offers a web interface access to it's logging output on the standard HTTP port 80. For example, your first need to find the IP address or DNS name of the device from your router and use one of the following addresses in your browser:

- `http://10.0.0.133` (the IP address is an example here)
- `http://blindsv2.local` (default DNS address)
