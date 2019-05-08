# Internal navigation using WiFi

![](./static/relativity.jpg)

## Problem Statement

GPS signals cannot reach indoors and are imprecise, so they’re not suitable for positioning and navigation indoors. However, as WiFi grows ubiquitous, and as WiFi routers transmit a unique ID in the form of Mac address, we can use them to acquire location while indoors.

If we can acquire position indoors, we can guide people or robots through pathways and corridors, for example, in a mall, a school, or a big building. With a central monitoring system, the same system can be used to locate patients in a hospital or pets, and children in homes, or find where people are in case of fire hazards or other natural disasters in malls, etc.

In this project, we determine the indoor location of a device by fingerprinting the signal strength of wifi routers around it.


## Solution

All WiFi and Bluetooth accessories have a universally unique ID called Mac address. The Mac address is transmitted along with the network name by every router. As generally routers are fixed, and the range of the WiFi signal is limited, we can look at the available access points and estimate where we might be.

For example, here is a list of accesspoints scanned from my room.
- 38:17:c3:b5:bb:20,-79
- 38:17:c3:b5:bb:21,-79
- 38:17:c3:b5:bb:30,-85
- 38:17:c3:b5:bb:31,-85
- 38:17:c3:b5:bb:40,-89
- 38:17:c3:b5:bb:50,-74
- 38:17:c3:b5:bb:51,-75
- 38:17:c3:b5:bb:60,-52
- 38:17:c3:b5:bb:61,-52
- 38:17:c3:b5:bb:70,-55
- 38:17:c3:b5:bb:71,-55
- 38:17:c3:b5:c1:60,-78

And here are routers when scanned from Lab315
- 38:17:c3:b5:9b:56,-85
- 38:17:c3:b5:9b:55,-86
- 38:17:c3:b5:9b:52,-86
- 38:17:c3:b5:9b:51,-85
- 38:17:c3:b5:9b:50,-85
- 38:17:c3:b5:b1:d6,-75
- 38:17:c3:b5:b1:d4,-75
- 38:17:c3:b5:b1:d2,-75
- 38:17:c3:b5:b1:d1,-75
- 38:17:c3:b5:b1:d0,-75
- 38:17:c3:b5:9e:46,-85
- 38:17:c3:b5:c1:86,-78
- 38:17:c3:b5:c1:84,-78
- 38:17:c3:b5:c1:82,-79
- 38:17:c3:b5:9e:40,-83
- 10:da:43:e2:ef:d4,-30
- 38:17:c3:b5:cb:61,-68
- 38:17:c3:b5:cb:60,-68
- 38:17:c3:b5:cb:66,-71
- 38:17:c3:b5:cb:64,-71
- 38:17:c3:b5:cb:62,-68
- 38:17:c3:b5:b1:c2,-81
- 38:17:c3:b5:b1:c1,-81
- 38:17:c3:b5:b1:c0,-81
- 38:17:c3:b5:cb:22,-40
- 38:17:c3:b5:cb:26,-52
- 38:17:c3:b5:cb:24,-52
- 38:17:c3:b5:cb:21,-52
- 38:17:c3:b5:cb:20,-52
- 38:17:c3:b5:b8:76,-87
- 38:17:c3:b5:b8:74,-86
- 38:17:c3:b5:9e:56,-77
- 38:17:c3:b5:9e:55,-77
- 38:17:c3:b5:9e:52,-78
- 38:17:c3:b5:9e:51,-78
- 38:17:c3:b5:9e:50,-78
- 38:17:c3:b5:cb:76,-62
- 38:17:c3:b5:cb:74,-62
- 38:17:c3:b5:cb:72,-63
- 38:17:c3:b5:cb:71,-63
- 38:17:c3:b5:cb:70,-63
- 38:17:c3:b5:cb:36,-61
- 38:17:c3:b5:cb:34,-61
- 38:17:c3:b5:cb:32,-61
- 38:17:c3:b5:cb:31,-61
- 38:17:c3:b5:cb:30,-61

Notice that the macIDs are completely different. Even when locations are close by, the macIDs may be same, but the signal strengths differ. When trying to acquire the position, we scan the available routers again, and compare it with the already collected training data and get the location. We can use Baye's theorem for this.


## Challenges

- When a new WiFi point is added, it may make the prediction inaccurate. So we ignore data from macIDs not present in our training data
- As MacIDs are permanent and universally unique, storing them directly is a privacy risk. So we store the hashes of macID instead.
- SQL databases cant have table columns starting with numerals, so we have to convert the hash into a string and append it to an alphabet to ensure it never starts as a number.
  For example, instead of storing '00:0a:95:9d:68:16' directly,  we store 'a' + str(hash('00:0a:95:9d:68:16')) which is 'a8875810788082014848'.

## Results
I collected data over 4 locations with a duration of 10 minutes and sampling every second. So I have 2400 data points in total, 600 at every location.

70 percent of the data (1680 samples, 420 per location) were used for training, to find the probabilities for the Baye's theorem, and the remaining 30 percent was used for testing.

The accuracy over the entire data set was 89 percentage. The reduction in percentage was because two of the locations were very close and separated only by a glass wall. Glass doesn't reduce the wifi signal strength, so the finger prints of both the rooms would look very similar.

To store the data in the server and to retrieve the users location, I created a server and client program. The client program scans for WiFi and Bluetooth signals and sends it to the server, and the server processes them using Baye's theorem. As the server works on HTTP, a browser can be used to retrieve the location.

The working of Baye's theorem is explained in this [slide](https://docs.google.com/presentation/d/1EHMYnhgR93dLNwHQTafD7n8SyEZllggVjRno2TJnGAs/edit?usp=sharing)

## Further work

The method described in this paper works well and has low requirement for processing power, but there is room for improvement and there are some limitations to the technology:

- Bluetooth can also be added to increase accuracy.

- Since we are using RSSI for fingerprinting, and RSSI varies a lot, we cannot acquire a really precise location. In my case, I had a precision of 5.5 metres. The radius increases the more the RSSI varies. Generally, newer versions of WiFi use beamforming and that should stabilise the RSSI value.

- If the device used for collection of data and the one used for location acquisition have widely different radios, the localisation may be unreliable. A calibration step using a standard router can be needs to be done for more accuracy.

- In my project, I've ignored the different technologies used by the routers. The latest 5th gen routers have WiFi 802.11 AC technology that uses beamforming. The previous generation 802.11 N doesn't have beamforming. Beamforming improves RSSI, so when scanning the technology of both the router and the scanning device must be noted and used in training. If the training is done with data from a 802.11 N device, then newer devices with 802.11 AC beamforming may not be able to get their location accurately.

- More machine learning techniques can be used to classify data. If we train the data with several techniques, and test it, we'd know the informedness of each algorithm (Also called Youden's J statistic). We can predict the location from all the algorithms and use the relative accuracy to get a combined predicton. This could improve the accuracy a little more.

- This methods needs the device to scan periodically and can send signals. This can drain the battery. Instead, we can passively locate the devices by listening to the WiFi broadcast packets. All WiFi devices periodically broadcast their existence to all devices on the network they are connected to. By sniffing these packets, we can estimate the location at no additional cost to the device. However, this method is bad for privacy and has lower accuracy and lower frequency at which the device location can be acquired.

- A dashboard kind of system can be developed which can monitor the locations of all devices. When the client program is implemented in a device like ESP8266, the device could be programmed to periodically wake up and transmit location. Such a device could last for months without needing a battery replacement, and such a system could be used to track patients, children or pets.