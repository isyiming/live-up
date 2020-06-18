设计模式的六大原则
1、开闭原则（Open Close Principle）
  开闭原则的意思是：对扩展开放，对修改关闭。在程序需要进行拓展的时候，不能去修改原有的代码，实现一个热插拔的效果。简言之，是为了使程序的扩展性好，易于维护和升级。想要达到这样的效果，我们需要使用接口和抽象类，后面的具体设计中我们会提到这点。
2、里氏代换原则（Liskov Substitution Principle）
  里氏代换原则是面向对象设计的基本原则之一。 里氏代换原则中说，任何基类可以出现的地方，子类一定可以出现。LSP 是继承复用的基石，只有当派生类可以替换掉基类，且软件单位的功能不受到影响时，基类才能真正被复用，而派生类也能够在基类的基础上增加新的行为。里氏代换原则是对开闭原则的补充。实现开闭原则的关键步骤就是抽象化，而基类与子类的继承关系就是抽象化的具体实现，所以里氏代换原则是对实现抽象化的具体步骤的规范。
3、依赖倒转原则（Dependence Inversion Principle）
  这个原则是开闭原则的基础，具体内容：针对接口编程，依赖于抽象而不依赖于具体。
4、接口隔离原则（Interface Segregation Principle）
  这个原则的意思是：使用多个隔离的接口，比使用单个接口要好。它还有另外一个意思是：降低类之间的耦合度。由此可见，其实设计模式就是从大型软件架构出发、便于升级和维护的软件设计思想，它强调降低依赖，降低耦合。
5、迪米特法则，又称最少知道原则（Demeter Principle）
  最少知道原则是指：一个实体应当尽量少地与其他实体之间发生相互作用，使得系统功能模块相对独立。
6、合成复用原则（Composite Reuse Principle）
  合成复用原则是指：尽量使用合成/聚合的方式，而不是使用继承。


1. 创建型模式
  单例模式（Singleton Pattern）
      Windows 是多进程多线程的，在操作一个文件的时候，就不可避免地出现多个进程或线程同时操作一个文件的现象，所以所有文件的处理必须通过唯一的实例来进行。
  工厂模式（Factory Pattern）
      我更愿意称之为接口。设计一个连接服务器的框架，需要三个协议，"POP3"、"IMAP"、"HTTP"，可以把这三个作为产品类，共同实现一个接口。这个接口就是一个工厂。
  抽象工厂模式（Abstract Factory Pattern）
      或许接口本身也是需要一个工接口的。
  建造者模式（Builder Pattern）
      使用多个固定的零部件，搭配出不同的组合。肯德基菜单。
  原型模式（Prototype Pattern）
      用于创建重复的对象，如果一个对象直接创建时构造函数调用代价较大，那么可以通过复制一个现有的对象实例来创建新的对象。但是要考虑到浅拷贝和深拷贝。

2. 结构型模式
适配器模式（Adapter Pattern）
    读卡器作为内存卡与电脑之间的接口。适配器模式用于：已经存在的一个成熟的类，但是它的接口和需求不匹配，这时就需要一个适配器来对接。
桥接模式（Bridge Pattern）

过滤器模式（Filter、Criteria Pattern）

组合模式（Composite Pattern）

装饰器模式（Decorator Pattern）
外观模式（Facade Pattern）
    为复杂的系统提供外界访问的模块。
享元模式（Flyweight Pattern）
代理模式（Proxy Pattern）

3. 行为型模式
责任链模式（Chain of Responsibility Pattern）
    避免请求发送者与接收者耦合在一起，让多个对象都有可能接收请求，将这些对象连接成一条链，并且沿着这条链传递请求，直到有对象处理它为止。
    拦截的类都实现统一接口。
命令模式（Command Pattern）
    行为请求者和行为实现者之间的控制通过命令来实现。其实就是保证来请求者的可能请求组合拓展性，方便更改。因为行为请求可能是很多个命令的组合。比如，先插入，再复制，再删除。这么多的行为组合可能在应用中随时更改。
解释器模式（Interpreter Pattern）
迭代器模式（Iterator Pattern）
    事先为系统中的全部元素设计一个迭代器。其实就是将全部元素名或者访问方式包含进来。所以可以自己定义迭代顺序。所谓迭代，就是.next
中介者模式（Mediator Pattern）
    中介者模式（Mediator Pattern）是用来降低多个对象和类之间的通信复杂性。这种模式提供了一个中介类，该类通常处理不同类之间的通信，并支持松耦合，使代码易于维护。中介者模式属于行为型模式。

备忘录模式（Memento Pattern）
观察者模式（Observer Pattern）
状态模式（State Pattern）
空对象模式（Null Object Pattern）
策略模式（Strategy Pattern）
模板模式（Template Pattern）
访问者模式（Visitor Pattern）

4. J2EE 模式
MVC 模式（MVC Pattern）
业务代表模式（Business Delegate Pattern）
组合实体模式（Composite Entity Pattern）
数据访问对象模式（Data Access Object Pattern）
前端控制器模式（Front Controller Pattern）
拦截过滤器模式（Intercepting Filter Pattern）
服务定位器模式（Service Locator Pattern）
传输对象模式（Transfer Object Pattern）
