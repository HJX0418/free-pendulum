# free-pendulum
第一次提交：完成垂直+30°定点照射
第二次提交：15°定点尝试失败
    猜测原因如下：
    pulse1 = 300-(20.0/9.0)*data[pitch1+26];	
    data[150] 数组变大，而pulse1的取值没有改变，导致无法指向初始状态