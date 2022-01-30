## 错误处理`*`

> 错误处理是任何语言都需要考虑到的问题，而 Go 语言在错误处理上解决得更为完善，优雅的错误处理机制是 Go 语言的一大特点。

#### error

Go 语言引入了一个错误处理的标准模式，即 `error` 接口，该接口定义如下：

```go
type error interface {
    Error() string
}
```

对于大多数函数，如果要返回错误，可以将 `error` 作为多返回值的最后一个：

```go
func foo(param int)(ret int, err error)
{
  ...
}
```

调用时的代码：

```go
n, err := foo(0)
if err != nil {
    //  错误处理
} else {
    // 使用返回值n
}
```

我们还可以自定义错误类型，创建源文件 `error.go`，输入以下代码：

```go
package main

import "fmt"
import "errors"

//自定义的出错结构
type myError struct {
    arg  int
    errMsg string
}
//实现Error接口
func (e *myError) Error() string {
    return fmt.Sprintf("%d - %s", e.arg, e.errMsg)
}

//两种出错
func error_test(arg int) (int, error) {
    if arg < 0  {
         return -1, errors.New("Bad Arguments - negtive!")
     }else if arg >256 {
        return -1, &myError{arg, "Bad Arguments - too large!"}
    }
    return arg*arg, nil
}

//相关的测试
func main() {
    for _, i := range []int{-1, 4, 1000} {
        if r, e := error_test(i); e != nil {
            fmt.Println("failed:", e)
        } else {
            fmt.Println("success:", r)
        }
    }
}
```