## BLINDS V2 433Mhz Controller

This is an example of a blinds 433Mhz based controller. It receives commands from the MQTT Broker and transmit control commands to blinds through a RF 433Mhz transmitter.

This version is now being built using the ESPHome open-source system. You must have installed it on your computer using the installation procedure available at the following [location](https://esphome.io/guides/installing_esphome.html).

The application requires the presence of a file named `secrets.yaml` located in the main application folder. To do so, please

1. Update the supplied `secrets.example.yaml` file with your network configuration parameters
2. Rename it to `secrets.yaml`

Use the following commands in a terminal window to compile and upload the application to the ESP32-based controller. After moving to the application folder, do the following:

```
esphome compile main.yaml
esphome upload main.yaml
```
