package main

import (
	"fmt"
	"sync"
	"time"
	"strings"
	"github.com/astaxie/beego/httplib"
)

var (
	Value float64 = 0.0
	msgChan chan string = make(chan string, 1000000)

	Count int64 = 0
	mutex sync.Mutex
)

func main() {
	for i := 0; i<50; i++ {
		go worker()
	}
	go gen()
	for {
		time.Sleep(time.Duration(2) * time.Second)
		mutex.Lock()
		if Count >= 9999000 {
			mutex.Unlock()
			break
		}
		mutex.Unlock()
	}
}

func worker() {
	strArr := make([]string, 0, 100)
	for {
		str := <-msgChan
		strArr = append(strArr, str)
		if len(strArr) == 100 {
			insertBatch(strArr)
			strArr = make([]string, 0, 100)
			mutex.Lock()
			Count += 100
			mutex.Unlock()
		}
	}
}

func insertBatch(row []string) {
	req := httplib.Post("http://localhost:8086/write?db=test")
	httpReq := req.GetRequest()
	httpReq.Close = true
	str := strings.Join(row, "\n")
	req.Body([]byte(str))

	_, err := req.String()
	if err != nil {
		fmt.Printf("err: %v\n", err)
	}
}

func gen() {
	count := 0
	for i := 1467000000; i < 1467010000; i++ {	// time
		for j := 1; j <= 100; j++ {	// host_id
			for k := 1; k <= 10; k++ { // metric
				count++
				if count % 10000 == 0 {
					fmt.Printf("count: %d\n", count)
				}
				str := fmt.Sprintf("cpu_load%d,host_id=%d value=%f %d000000000", k, j, GetValue(), i)
				msgChan <- str
			}
		}
	}
}

func GetValue() float64 {
	Value += 0.1
	if Value >= float64(1) {
		Value = 0
	}
	return Value
}
