#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <queue>
#include <atomic>
#include <functional>
#include <thread>
#include <future>  // 类模板 std::future 提供访问异步操作结果的机制
#include <stdexcept>

namespace Pool
{
// 线程池容量
#define THREADPOOL_MAX_NUM 16

// 线程池,可以提交变参函数或lamdba表达式的匿名函数执行,可以获取执行返回值
// 不直接支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
class ThreadPool {
    // std::function call a method:
    // - use bind
    //   std::function<void()> doThing = std::bind(&type::isMethod, &object);
    // - use Lambda
    //   std::function<void()> doThing = [&]{object.isMethod();};
    // https://stackoverflow.com/a/27321603
    // https://www.cnblogs.com/yinwei-space/p/12708871.html
    using Task = std::function<void()>; // 定义类型
    std::vector<std::thread> _pool; // 线程池
    std::queue<Task> _tasks;  // 任务队列
    std::mutex _lock;  // 同步
    std::condition_variable _task_cv;  // 条件阻塞

    std::atomic<bool> _run{true};  // 线程池是否执行
    std::atomic<int> _idThrNum{0};  // 空闲线程数量

public:
    // 因为inline同register一样，只是个建议，编译器并不一定真正的内联，在语法上是没有错误的
    inline ThreadPool(unsigned short size = 4) { addThread(size);}
    inline ~ThreadPool() {
        _run = false;
        _task_cv.notify_all();  // 唤醒所有线程执行
        for (auto &iter: _pool) {
            if (iter.joinable()) {
                iter.join();
            }
        }
    }

public:

	// 提交一个任务
	// 调用.get()获取返回值会等待任务执行完,获取返回值
	// 有两种方法可以实现调用类成员，
	// 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
	// 一种是用   mem_fn： .commit(std::mem_fn(&Dog::sayHello), this)
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        if (!_run) {
            throw std::runtime_error("commit on ThreadPool is stopped.");
        }

        using RetType = decltype(f(args...));  // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
        // std::packaged_task 包装任何可调用 (Callable) 目标（函数、 lambda 表达式、 
        // bind 表达式或其他函数对象），使得能异步调用它。其返回值或所抛异常被存储于能通过 std::future 对象访问的共享状态中。
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );  // 把函数入口及参数,打包(绑定)

        std::future<RetType> future = task->get_future();

        // Control variable scope.
        // Variables with automatic storage are destroyed when they go out of scope.
        // This can also enable a destructor to be called earlier than it otherwise would.
        // https://stackoverflow.com/a/9704107
        // Historical note: This is a technique from the early C language which allowed creation of local temporary variables.
        // https://blog.csdn.net/weixin_45360983/article/details/100124312?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-2.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-2.no_search_link
        {
            // 添加任务到队列中。
            std::lock_guard<std::mutex> lock{_lock};  //对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
            _tasks.emplace([task](){  //  push(Task{...}) 放到队列后面
                (*task)();
            });
        }

#ifdef THREADPOOL_AUTO_GROW
        if (_idThrNum < 1 && _pool.size() < THREADPOOL_MAX_NUM)
            addThread(1);
#endif
        _task_cv.notify_one();  // 唤醒一个线程执行
        return future;
    }

    // 空闲线程数量
    int idleCount() { return _idThrNum; }

    // 线程数量
    int thrCount() { return _pool.size(); }

#ifndef THREADPOOL_AUTO_GROW
private:
#endif // !THREADPOOL_AUTO_GROW

    // 添加指定线程数量
    void addThread(unsigned short size) {
        for (; _pool.size() < THREADPOOL_MAX_NUM && size > 0; --size) {
            //增加线程数量,但不超过 预定义数量 THREADPOOL_MAX_NUM
            _pool.emplace_back([this]{
                while (_run) {
                    Task task;  // 获取一个待执行的 task
                    {
                        // unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
                        std::unique_lock<std::mutex> lock{_lock};
                        _task_cv.wait(lock, [this]{
                            return !_run || !_tasks.empty();
                        });  // wait 直到有 task
                        if (!_run && _tasks.empty())
                            return;
                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }
                    _idThrNum--;
                    task();
                    _idThrNum++;
                }
            });
            _idThrNum++;
        }
    }

};


} // namespace Pool


#endif  // _THREAD_POOL_H_