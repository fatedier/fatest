package main

import (
	"encoding/json"
	"fmt"
	"sync"
	"time"

	"github.com/astaxie/beego/httplib"
)

type Metric struct {
	Name      string            `json:"metric"`
	Timestamp int64             `json:"timestamp"`
	Value     float64           `json:"value"`
	Tags      map[string]string `json:"tags"`
}

var (
	Value   float64      = 0.0
	msgChan chan *Metric = make(chan *Metric, 1000000)

	Count int64 = 0
	mutex sync.Mutex
)

func init() {
}

func main() {
	for i := 0; i < 50; i++ {
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
	metricArr := make([]*Metric, 0, 100)
	for {
		metric := <-msgChan
		metricArr = append(metricArr, metric)
		if len(metricArr) == 100 {
			insertBatch(metricArr)
			metricArr = make([]*Metric, 0, 100)
			mutex.Lock()
			Count += 100
			mutex.Unlock()
		}
	}
}

func insertBatch(metrics []*Metric) {
	req := httplib.Post("http://localhost:4242/api/put?summary")
	httpReq := req.GetRequest()
	httpReq.Close = true
	str, _ := json.Marshal(metrics)
	req.Body([]byte(str))

	_, err := req.String()
	if err != nil {
		fmt.Printf("err: %v\n", err)
	}
}

func gen() {
	count := 0
	for i := 1467000000; i < 1467010000; i++ { // time
		for j := 1; j <= 100; j++ { // host_id
			for k := 1; k <= 10; k++ { // metric
				count++
				if count%10000 == 0 {
					fmt.Printf("count: %d\n", count)
				}
				metric := &Metric{
					Name:      fmt.Sprintf("cpu_load%d", k),
					Timestamp: int64(i),
					Value:     GetValue(),
					Tags:      make(map[string]string),
				}
				metric.Tags["host_id"] = fmt.Sprintf("%d", j)
				msgChan <- metric
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
