#include "Threadpool.h"
#include "./Exception.h"

namespace oar {

  Threadpool::Threadpool(int threadNums, const std::string name,
			 ThreadpoolInitCb task)
    :_threadNums(threadNums),
     _maxTaskSize(5),
     _name(name),
     _mutex(),
     _notfull(_mutex),
     _notempty(_mutex),
     _threads(),
     _tasks(),
     _running(false),
     _stopRunNewTask(false),
     _initCallBack(task)
  {
    
  }

  Threadpool::~Threadpool() {
    if (_running)
      stop();

    //printf("~~~~~~~~\n");
  }

  void Threadpool::start() {
    assert(_threads.empty());
    _running = true;
    _threads.reserve(_threadNums);
    for (int i = 0; i < _threadNums; ++i) {
      _threads.emplace_back(new oar::Thread(&Threadpool::loopInThread, this));
      _threads[i]->start();
    }
    if (_initCallBack)
      _initCallBack(_name);
  }

  void Threadpool::stop() {
    {
      MutexGuard mg(_mutex);
      _running = false;
      _notempty.broadcast();
      _notfull.broadcast();
    }
    everyThreadJoin();
  }
  // 将当前任务队列的任务做完后结束
  void Threadpool::stopRunNewTask() {
    {
      MutexGuard mg(_mutex);
      _stopRunNewTask = true;
      _notempty.broadcast();
      _notfull.broadcast();
      _running = false;
    }
    everyThreadJoin();
    //    _running = false;
  }
  
  void Threadpool::runTask(Task task) {
    //    printf("Threadpool::runTask current tasks size is %d\n",taskQueuesize());
    //    if (_stopRunTask)
    //return;
    assert(!_stopRunNewTask);
    if (_threads.empty())
      task();
    else {
      MutexGuard mg(_mutex);
      while (_running && isFull()) {
	_notfull.wait(); //_tasks满时等待
      }
      //      printf("notfull wait() end\n");
      if (!_running)
	return;
      assert(!isFull());
      _tasks.push_back(std::move(task));
      //      printf("push down......\n");
      _notempty.notify();
    }
  }
  
  size_t Threadpool::taskQueuesize() const {
    MutexGuard mg(_mutex);
    return _tasks.size();
  }

  void Threadpool::setTaskQueueSize(size_t size) {
    if (size > taskQueuesize())
      _tasks.resize(size);
  }

  void oar::Threadpool::loopInThread() {
    try {
      // =======================TODO
      // 这里应该调用Thread的initcallback
      //if (_initCallBack)
      //	_initCallBack();

      // 调用stop()直接停止
      // 调用stopRunNewtask()则将已在队列的任务执行完再结束
      // _running必须在后面，否则判断_running为false后则不会继续判断||后的东西，即使为true
      while ((!_tasks.empty() && _stopRunNewTask) || _running) {
	Task task = take();
	if(task)
	  task();
	//	printf("empty = %d _stoprunnewtask = %d running = %d\n",_tasks.empty(),_stopRunNewTask,_running);
      }
    } catch (const Exception& e) {
      fprintf(stderr, "Exception caught in Threadpool %s\n", _name.c_str());
      fprintf(stderr, "reason: %s\n", e.what());
      fprintf(stderr, "stacktrace: %s\n", e.stackTrace());
      abort();
    } catch (std::exception& e) {
      fprintf(stderr, "Exception caught in Threadpool %s\n", _name.c_str());
      fprintf(stderr, "reason: %s\n", e.what());
      abort();
    } catch (...) {
      fprintf(stderr, "unknown exception caught in Threadpool %s\n", _name.c_str());
      throw;
    }
  }
  
  void oar::Threadpool::everyThreadJoin() {
    for (auto& t : _threads) {
      t->join();
      //printf("join............\n");
    }
  }

  bool oar::Threadpool::isFull() const {
    if (!_mutex.LockedByThisThread())
      return false;
    return _maxTaskSize > 0 && _tasks.size() >= _maxTaskSize;
  }

  oar::Threadpool::Task oar::Threadpool::take() {
    MutexGuard mg(_mutex);
    while (_running && _tasks.empty()) {
      _notempty.wait();
    }
    Task task;
    if(!_tasks.empty()) {
      task = _tasks.front();
      if (task) {
	_tasks.pop_front();
      }
      // 取完一个任务后将runTask中wait的条件变量唤醒
      if (_maxTaskSize > 0)
	_notfull.notify();
    }
    return task;
  }
}
