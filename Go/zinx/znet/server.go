
package znet
import (
    "fmt"
    "net"
    "zinx/ziface"
)

// IServer 接口实现，定义一个Server服务类
type Server struct {
	// 服务器的名称
	Name string

	IPVersion string

	IP string

	Port int
}

/*
	创建一个服务器句柄
*/

func NewServer (name string) ziface.IServer  {
	s := &Server {
		Name: name,
		IPVersion:"tcp4",
		IP:"0.0.0.0",
		Port:7777,
	}
	return s
}

// 开启网络服务
func (s *Server) Start() {
	fmt.Printf("[START] Server listenner at IP: %s, Port %d, is starting\n", s.IP, s.Port)

	// 开启一个go去做服务端Linster业务
	go func() {
		// 1. 获取一个tcp的Addr
		addr, err := net.ResolveTCPAddr(s.IPVersion, fmt.Sprintf("%s,%d", s.IP, s. Port))
		if err != nil {
			fmt.Println("resolve tcp addr err: ", err)
		}

		// 2. 监听服务器地址
		listenner, err := net.ListenTCP(s.IPVersion, addr)
		if err != nil {
			fmt.Println("listen", s.IPVersion, "err", err)
			return
		}

		// 已经监听成功
		fmt.Println("Start zinx Server ", s.Name, " succ, now listenning……")

		// 3 启动server网络连接业务
		for {
			// 3.1 阻塞等待客户端建立连接请求
			conn, err := listenner.AcceptTCP()
			if err != nil {
				fmt.Println("Accept err ", err)
				continue
			}

			// 3.2 TODO Server.Start() 设置服务器最大连接控制，如果超过最大连接，那么关闭此新的连接
			// 3.3 TODO Server.Start() 处理该新连接请求的 *业务* 方法，此时应该有handler和conn是绑定的
			// 我们这里暂时做一个最大512字节的回显服务
			go func() {
				// 不断地循环从客户端获取数据
				for {
					buf := make([]byte, 512)
					cnt, err := conn.Read(buf)
					if err != nil {
						fmt.Println("recv bug err ", err)
						continue
					}
					
					// 回显
					if _, err := conn.Write(buf[:cnt]); err != nil {
						fmt.Println("write back buf err", err)
						continue
					}
				}
			}()
		}
	}()
}

// 停止
func (s *Server) Stop() {
	fmt.Println("[STOP] Zinx server, name", s.Name)
}

// 开启业务
func (s *Server) Server()  {
	
}
  