# `panic` 和 `recover`

`panic()` 函数用于抛出异常，`recover()` 函数用于捕获异常，这两个函数的原型如下：

```go
func panic(interface{})
func recover() interface{}
```

当在一个函数中调用 `panic()` 时，正常的函数执行流程将立即终止，但函数中之前使用 `defer` 关键字延迟执行的语句将正常展开执行，之后该函数将返回到调用函数，并导致逐层向上执行 `panic()` 流程，直至所属的 `goroutine` 中所有正在执行的函数被终止。错误信息将被报告，包括在调用 `panic()` 函数时传入的参数，这个过程称为错误流程处理。

`panic()` 接受一个 `interface{}` 参数，可支持任意类型，例如：

```go
panic(404)
panic("network broken")
panic(Error("file not exists"))
```

在 `defer` 语句中，可以使用 `recover()` 终止错误处理流程，这样可以避免异常向上传递，但要注意 `recover()` 之后，程序不会再回到 `panic()` 那里，函数仍在 `defer` 之后返回。新建一个源文件 `error1.go`，输入以下代码：

```go
func foo() {
    panic(errors.New("i'm a bug"))
    return
}

func test() (result int) {
    defer func() {
        if r := recover(); r != nil {
            err := r.(error)
            fmt.Println("Cache Exception:", err)
        }
    }()
    foo()
    return 10
}

func main() {
    fmt.Println(test())     // 0
}
```

