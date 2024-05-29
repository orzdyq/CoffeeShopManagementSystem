/*
项目 1：咖啡店订单管理系统123
任务说明：使用 C 语言设计开发一个简单的咖啡店订单管理系统，实现商
品及订单的添加、删除、修改、查询等功能。

（1） 数据结构
程序中采用结构体存储商品和订单信息，用链表或数组保存多个商品和订单
信息。商品信息结构体成员包括：商品编号、名称、价格、描述（如配料、热量等）、商品分类、商品库存等。订单信息结构体成员包括：订单号、顾客信息、订单时间、订单状态（已完成、未完成等）以及订单中的商品项列表等。

（2） 软件功能说明
1） 商品信息管理功能：a. 显示所有信息：按照类别显示所有商品的信息。
b. 添加商品信息：从键盘输入商品的基本信息，并将其添加到系统中。c. 删除商品信息：根据商品编号或其他关键信息删除商品的信息。d. 修改商品信息：根据商品编号或其他关键信息修改商品的基本信息。e. 查找商品信息：根据商品编号或其他关键信息查找商品，并显示其详细信息。
2） 订单信息管理功能：a. 创建订单：选择商品并指定数量，生成订单号，
将订单添加到系统中。b. 取消订单：根据订单号取消订单，并将订单状态设置为取消。c. 完成订单：根据订单号将订单状态设置为已完成。d. 显示所有订单信息：按照订单时间，逐个显示所有订单的信息。e. 查找订单信息：根据订单号或其他关键信息查找订单，并显示其详细信息。
3） 实现财务统计功能：a. 统计某段时间内的总销售额。b. 统计某段时间内
每种商品的销售数量和销售。c. 保存统计信息到文件等4） 其他功能：登录、退出系统；顾客星级以及优惠设置等。

相关小组开发的系统，可以包含但不限于上述功能，需要自行设计更具有实
用性和创新性的系统功能。完成系统设计和开发后，请按附件 1 实验报告模板按要求完成报告内容并以附件形式提交。
*/
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define bool int
#define true 1
#define false 0
#define POS_X1 35 // 1、菜单页，第一列所有功能输出起始位置的x坐标值；2、排序之后输出提示起点的X坐标值
#define POS_X2 40 // 输入模块的提示语句起点的X坐标值
#define POS_X3 50 // 第一次，调用输入模块或磁盘数据读入模块功能以外的其他功能，提示“系统中尚无学生成绩信息，请先输入”的起始位置的X坐标值
#define POS_X4 65 // 菜单页，第二列所有功能输出起始位置的x坐标值；
#define POS_Y 3   // 排序之后输出提示起点的Y坐标值

// 结构体定义
// 商品
typedef struct product
{
    long productID;
    char productName[20];
    double price;
    char description[50]; // 商品描述
    int inventory;        // 库存数量
} Product;

// 订单
typedef struct order
{
    long orderID;
    // TODO Customer……
    time_t orderTime; // 订单时间，使用time_t类型，可以通过time(NULL)获取当前时间
    // char orderTime[20];       // 订单时间
    int status;              // 订单状态，0表示未完成，1表示已完成，2表示已取消
    int productNum;          // 订单中商品项的数量
    Product productList[20]; // 订单中包含的商品项列表
    double totalPrice;       // 订单总价
    // int customerID;           // 顾客ID，用于关联顾客信息，如积分、等级等
} Order;

// 自定义函数声明
int PrintMenu();                                    // 生成菜单
void SetPosition(int x, int y);                     // 设置输出内容在控制台窗口中的起始位置
void InputProduct(int *productNum, Product *pro);   // 输入商品信息
void AppendProduct(int *productNum, Product *pro);  // 增加商品信息
void DeleteProduct(int *productNum, Product *pro);  // 删除商品信息，根据商品编号或其他关键信息删除商品的信息
void ModifyProduct(int *productNum, Product *pro);  // 修改商品信息，根据商品编号或其他关键信息修改商品的基本信息
void DisplayProduct(int *productNum, Product *pro); // 显示商品信息，按照类别显示所有商品的信息
void SearchProduct(int *productNum, Product *pro);  // 查找商品信息，根据商品编号或其他关键信息查找商品，并显示其详细信息

void AppendOrder(int *productNum, int *orderNum, Product *pro, Order *ord); // 增加订单信息，选择商品并指定数量，生成订单
void DeleteOrder(int *productNum, int *orderNum, Order *ord);               // 删除订单信息，根据订单号取消订单，并将订单状态设置为取消
void ModifyOrder(int *productNum, int *orderNum, Order *ord);               // 修改订单信息
void DisplayOrder(int *productNum, int *orderNum, Order *ord);              // 显示订单信息，按照订单时间，逐个显示所有订单的信息
void SearchOrder(int *productNum, int *orderNum, Order *ord);               // 查找订单信息，根据订单号或其他关键信息查找订单，并显示其详细信息

void Statistics(int *productNum, int *orderNum, Order *ord);   // 财务统计功能
void SaveToFile(int *productNum, int *orderNum, Order *ord);   // 保存数据到文件，保存统计信息到文件等
void ReadFromFile(int *productNum, int *orderNum, Order *ord); // 从文件中读取数据，读取统计信息到文件等

int main()
{
    // 要用一个变量标记当前系统内有的商品or订单的数量
    int productNum = 0; // 商品数量计数器
    int orderNum = 0;   // 订单数量计数器
    Product pro[100];   // 商品列表，最多支持的商品数量
    Order ord[100];     // 订单列表，最多支持的订单数量

    int option;
    int first = 1; // 标记是否是第一次运行，若是（1），则要先输入商品信息
    system("mode con cols=130 lines=60");
    system("color 0E");
    while (true)
    {
        system("cls");
        option = PrintMenu();
        switch (option)
        {
        case 1:
            system("cls");
            InputProduct(&productNum, pro);
            first = 0; // 标记为非第一次
            break;
        case 2:
            system("cls");
            if (first)
            {
                printf("系统中尚无商品信息，请先输入！\n");
                system("pause");
                break;
            }
            AppendProduct(&productNum, pro);
            system("pause");
            break;
        case 7:
            system("cls");
            AppendOrder(&productNum, &orderNum, pro, ord);
            break;
        case 0:
            system("cls");
            SetPosition(0, 10);
            printf("退出系统！\n");
            exit(0);
        default:
            system("cls");
            SetPosition(0, 10);
            printf("输入出错，请重新选择操作！\n");
            system("pause");
        }
    }
    return 0;
}

void SetPosition(int x, int y)
{
    HANDLE hOut;
    COORD pos;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut, pos);
}

int PrintMenu()
{
    int posy = 5;
    int option; // 用户输入的操作
    int i, j;
    SetPosition(POS_X3, posy);
    printf("咖啡店订单管理系统\n"); // 输出系统名
    // 输出系统名和功能说明之间的两行短横线
    for (i = 0; i < 2; i++)
    {
        SetPosition(POS_X1, ++posy);
        for (j = 0; j < 55; j++)
        {
            printf("-");
        }
    }
    // 输出系统支持的功能和对应的功能代号
    SetPosition(POS_X1, ++posy);
    printf("1.输入商品信息");
    SetPosition(POS_X4, posy);
    printf("2.增加商品信息");
    SetPosition(POS_X1, posy += 2);
    printf("3.删除商品信息");
    SetPosition(POS_X4, posy);
    printf("4.修改商品信息");
    SetPosition(POS_X1, posy += 2);
    printf("5.显示商品信息");
    SetPosition(POS_X4, posy);
    printf("6.查找商品信息");
    SetPosition(POS_X1, posy += 2);
    printf("7.创建订单");
    SetPosition(POS_X4, posy);
    printf("8.取消订单");
    SetPosition(POS_X1, posy += 2);
    printf("9.完成订单");
    SetPosition(POS_X4, posy);
    printf("10.显示所有订单信息");
    SetPosition(POS_X1, posy += 2);
    printf("11.查找订单信息");
    SetPosition(POS_X4, posy);
    printf("12.财务统计");
    SetPosition(POS_X1, posy += 2);
    printf("13.保存数据到文件");
    SetPosition(POS_X4, posy);
    printf("14.从文件中读取数据");
    SetPosition(POS_X1, posy += 2);
    printf("0.退出");
    // 输出系统支持的功能与输入提示语之间的两行短横线
    for (i = 0; i < 2; i++)
    {
        SetPosition(POS_X1, ++posy);
        for (j = 0; j < 55; j++)
        {
            printf("-");
        }
    }
    // 提示用户输入所要执行的功能代号
    SetPosition(POS_X1, ++posy); // 好像没对齐？
    printf("请选择你想要进行的操作[0~16]: [  ]\b\b\b");
    scanf("%d", &option);
    return option;
}

// n是商品数量，pro是商品列表，pro[i]是商品列表中的第i个商品
void InputProduct(int *productNum, Product *pro)
{
    int i, j;
    int posy = 6;
    SetPosition(POS_X2, posy); // 设置光标位置，即需要显示的位置
    printf("输入商品数量:");
    scanf("%d", productNum);
    SetPosition(POS_X2, posy += 2); // 设置光标位置，即需要显示的位置
    for (i = 0; i < 2; i++)
    {
        SetPosition(POS_X1, ++posy);
        for (j = 0; j < 55; j++)
        {
            printf("-");
        }
    }
    SetPosition(POS_X2, ++posy);
    printf("请输入商品编号、名称、价格、描述和库存:");
    for (i = 0; i < *productNum; i++)
    {
        SetPosition(POS_X2, ++posy);
        printf("输入第%d个商品信息:\n", i + 1);
        // scanf("%ld%s%lf%s%d", &pro[i].productID, pro[i].productID, pro[i].price, pro[i].description, pro[i].inventory);

        // 逐个提示用户输入。注意打印位置
        SetPosition(POS_X2, ++posy);
        printf("商品编号:");
        scanf("%ld", &pro[i].productID);
        SetPosition(POS_X2, ++posy);
        printf("商品名称:");
        scanf("%s", pro[i].productName);
        SetPosition(POS_X2, ++posy);
        printf("商品价格:");
        scanf("%lf", &pro[i].price);
        SetPosition(POS_X2, ++posy);
        printf("商品描述:");
        scanf("%s", pro[i].description);
        SetPosition(POS_X2, ++posy);
        printf("商品库存:");
        scanf("%d", &pro[i].inventory);
    }
    SetPosition(POS_X2, posy += 2);
    printf("商品信息输入完成！");
    SetPosition(POS_X2, posy += 2);
    printf("按任意键继续...");
    getchar();
    getchar();
}

void AppendProduct(int *productNum, Product *pro)
{
    int i, j;
    int num; // 增加产品的数量
}

// 下面是订单

// 增加订单信息，选择商品并指定数量，生成订单
void AppendOrder(int *productNum, int *orderNum, Product *pro, Order *ord)
{
    // 先调用DisplayProduct，显示商品信息，相当于打印菜单
    // DisplayProduct();

    int choiceProductID; // 用户选择的商品编号
    int numPerProduct;   // 一种商品的购买数量
    int posy = 6;

    SetPosition(POS_X2, posy += 2);
    for (int i = 0; i < 2; i++)
    {
        SetPosition(POS_X2, ++posy);
        for (int j = 0; j < 55; j++)
        {
            printf("-");
        }
    }
    while (true)
    {
        int flag = 0;                       // 标记用户输入的商品编号对应的商品是否存在，1表示存在，0表示不存在
        if (ord[*orderNum].productNum == 0) // 订单中还没有商品项
        {
            SetPosition(POS_X2, posy += 2);
            printf("请输入要购买的商品编号(输入-1退出):");
        }
        else // 订单中已经有商品项
        {
            SetPosition(POS_X2, posy += 2);
            printf("请继续输入要购买的商品编号(输入-1退出):");
        }

        scanf("%d", &choiceProductID);
        if (choiceProductID == -1)
        {
            break; //-1退出
        }

        // 若没有此商品，提示异常。即商品列表中没有choiceProductID这个productID。也要考虑到productID不按照顺序的情况，所以每次需要遍历整个列表，查找有没有这个productID，如果有，就用choiceProductIDIndex来标记，不是减1
        int choiceProductIDIndex = -1; // 标记choiceProductID在商品列表中的下标，-1表示不存在
        for (int i = 0; i < *productNum; i++)
        {
            if (pro[i].productID == choiceProductID)
            {
                flag = 1;
                choiceProductIDIndex = i;
                break;
            }
        }
        if (flag == 0)
        {
            SetPosition(POS_X2, posy += 2);
            printf("没有此商品哦，请重新输入。\n");
            continue;
        }

        // 获取订单信息
        SetPosition(POS_X2, posy += 2);
        printf("请输入购买数量: ");
        scanf("%d", &numPerProduct);

        // 若库存不足，提示异常
        if (pro[choiceProductIDIndex].inventory < numPerProduct)
        {
            SetPosition(POS_X2, posy += 2);
            printf("\"%s\"的库存不足，请重新输入。\n", pro[choiceProductIDIndex].productName); // 商品编号从1开始，但是数组下标从0开始，所以要减1
            continue;
        }

        // 若库存充足，则将商品信息添加到订单中
        ord[*orderNum].productNum++;            // 商品项加一，意思是这个订单中商品有几种。但是每种商品的数量是对应的numPerProduct
        ord[*orderNum].orderID = *orderNum + 1; // 订单编号从1开始
        ord[*orderNum].orderTime = time(NULL);  // 订单时间
        ord[*orderNum].status = 0;              // 订单状态为未完成

        // Order结构体里的productList只用前三个成员变量
        ord[*orderNum].productList[0].productID = pro[choiceProductIDIndex].productID;
        strcpy(ord[*orderNum].productList[0].productName, pro[choiceProductIDIndex].productName);
        ord[*orderNum].productList[0].price = pro[choiceProductIDIndex].price;

        // 计算总价
        ord[*orderNum].totalPrice += pro[choiceProductIDIndex].price * numPerProduct;

        // 提示用户已成功添加订单
        SetPosition(POS_X2, posy += 2);
        printf("OK，已成功添加：%d份%s。\n", numPerProduct, pro[choiceProductIDIndex].productName);
    }
}
