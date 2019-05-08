package main

import (
	"fmt"
	"log"
	"os"

	wifiscan "github.com/zeroby0/wifiscan"
)

func main() {
	wifis, err := wifiscan.Scan(os.Getenv("WIFI"))
	if err != nil {
		log.Fatal(err)
	}
	if len(wifis) > 0 {
// 		fmt.Println("SSID\t\t\tRSSI")
	} else {
		fmt.Println("no mac addresses found")
	}
	for _, w := range wifis {
		fmt.Printf("%s,%d\n", w.SSID, w.RSSI)
	}
}
