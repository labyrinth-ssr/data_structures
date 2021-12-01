RB-TREE设计细节与思考：

类的设计：
考虑到一条数据较小直接放在一个类中，使用bool is_NIL()函数封装判断NIL（实际为判断key是否为空），增加可读性和封装性。

表示NIL的方法：
由于空指针的不稳定性，选择在node中enum为NIL，且不可将tree的NIL指定为一个指针，会导致在fix时使用同一NIL而出错。不需要单独管理tree.nil因为NIL是整个tree的一部分，可以通过指针找到。所以NIL动态分配，并且在插入后删除

内存管理：
考虑到数据量的不确定性，使用动态内存分配，且出于效率问题不使用STL管理内存。原本担心在init文件较大数据量时先统一分配一块大空间，避免所谓的频繁new与delete造成的效率问题与内存分配问题，实际实验发现问题不大，且init分配一大块内存与insert，update的内存管理不统一，更加难以delete。管理内存时注意new与delete发生在同一层即可。

异常处理：
key conflict,key missing使用throw 抛出异常，后在具体的功能函数中catch，进行处理。将异常与函数解耦。故delete可复用search，update可复用search和insert。

红黑树细节：
y是red时不用fix的原因：不影响红黑树的基本原则。
delete_fixup:double black,通过在这一分支增加黑色node解决为了保持black node数，可以随意将red转black而不用担心冲突。 

其他：
因为有一些函数的交互类似，做了统一的输入输出的处理freq_list_line_input（）。
friendly UI maybe:main函数中循环选择所使用的的功能。
其余函数实现与书上大致相同。

B-TREE

设计细节：
PageNode类是一个Node，包含大小n，keys数组，c（children）数组，bool is_leaf判断是否为叶节点。
BTree结构包含两个PageNode指针 root和NIL，NIL用于表示空节点，如search失败时的返回（保持形式统一不使用空指针）
dump，search出于方便考虑均使用递归
b_min_node与b_max_node用于寻找predecessor和successor，while实现。
delete_internal_key不同于删除，仅用于节点内部borrow sibiling时对sibiling的改变。
其余函数与书上内容大致相同。

内存管理：
为了简化问题，且satellitedata较小，将satellite data和key在一起。
由于PageNode大小有上下限，容量较为固定，PageNode中的key数组，child静态分配由类构造与析构自动管理，使用时依靠类中的n实现容量大小控制。

异常处理：
考虑到异常情况并不复杂，写太多catch try块可能更臃肿，直接在函数内使用cout return提示异常并结束函数。

friendly UI同上。

