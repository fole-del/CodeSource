package ziface

package ziface

import "go/types"

// 定义服务器接口
type IServer interface {
	// 启动服务器
	Start()
	// 停止服务器
	Stop()
	// 开启业务服务方法
	Server()
}