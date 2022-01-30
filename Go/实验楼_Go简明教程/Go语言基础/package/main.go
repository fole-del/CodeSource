package main

import (
	"add" //导入 add 包 这个包必须放在$GOPATH下才可
	"fmt"
)

func main() {
	c := add.Add(1, 2) //调用 add 包中的 add 函数
	fmt.Println(c)
}
