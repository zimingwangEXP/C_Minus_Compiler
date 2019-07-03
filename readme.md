**本次实验实现了一个简易的C_Minus编译器的部分功能（词法分析，语法分析）
词法分析使用了lex和手写两种方式分别实现
语法分析使用了递归下降，LL(1),SLR(1),yacc四种方式分别实现
部分实验结果如下(全部实验结果请参照实验报告及相应的文件)：**
&nbsp;
**一种正确输入：**
```
int x[10];
int minloc (int a[], int low, int high){
  int i;
  int x;
  int k;
  k = low;
  x = a[low];
  i = low + 1;
  while(i < high){
    if(a[i] < x){
      x = a[i];
      k = i;
    }
    i = i + 1;
  }
  return k;
}
void main(void){
	output(minloc(x, 4, 9));
}
```
**对应的输出结果：**
!["词法分析结果"](./md_resources/lexer_out.png)
<center> <h6>语法分析输出结果</h6> </center>
&nbsp;

!["使用的生成式列表"](./md_resources/production_list.png)
<center> <h6>使用的生成式列表</h6> </center>

&nbsp;

!["LL1分析表"](./md_resources/LL1_table.png)
<center> <h6>部分LL(1)分析表</h6> </center>

&nbsp;

!["LR0自动机"](./md_resources/LR0_Automata.png)
<center> <h6>部分LR(0)自动机</h6> </center>

&nbsp;

!["LR0分析表"](./md_resources/SLR1_table.png)
<center> <h6>部分SLR(1)分析表</h6> </center>

&nbsp;

!["LL1分析表"](./md_resources/LL1_table.png)
<center> <h6>部分LL1分析表</h6> </center>

**三种树输出格式如下:**
&nbsp;

!["缩进格式"](./md_resources/tree_in_incent.png)
<center> <h6>缩进格式输出的语法分析树</h6> </center>
&nbsp;

!["json格式"](./md_resources/tree_in_json.png)
<center> <h6>json格式输出的语法分析树</h6> </center>
&nbsp;

!["html"](./md_resources/tree_in_html.png)
<center> <h6>echarts绘制的语法分析树</h6> </center>

**一种错误的输入源程序如下:**
```
int x[10];
int minloc (int a[], int low, int high){
  int i;
  int x;
  int k;
  k = low;
  x = a[low;
  i = low + 1;
  while(i < high){
    if(a[] < x){
      x = a[i];
      k = i;
    }
    i = i + 1;
  }
  return k;
}
void main(void){
	output(minloc(x, 4, 9));
}
```
&nbsp;
**相应的输出：**
!["总体报错"](./md_resources/error_total.png)
!["建树"](./md_resources/errors_in_building_tree.png)
<center> <h6>报错信息</h6> </center>

**设计思路和方法详见编译原理课程设计报告：**

