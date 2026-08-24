#include <stdio.h> // 包含输入输出函数（如printf、scanf）
#include <stdlib.h> // 包含内存分配、程序退出等函数
#include <string.h>  // 包含字符串操作函数

// 学生信息结构体定义
typedef struct Student {
    int id;             // 学号（整数）
    char name[50];      // 姓名（最多50个字符的字符串）
    float score;        // 成绩（浮点数）
    struct Student* next;  // 指向下一个节点的指针（构成链表）
} Student;

// 全局变量
Student* head = NULL;  // 链表头指针，初始为空（无学生数据）
char filename[] = "students.dat";  // 存储数据的文件名

// 函数声明
Student* createStudent();// 创建新学生节点
void addStudent();// 添加学生到链表
void displayAll();// 显示所有学生信息
void searchStudent(); // 按学号搜索学生
void deleteStudent();// 按学号删除学生
void saveToFile(); // 保存数据到文件
void loadFromFile(); // 从文件加载数据
void freeList(); // 释放链表占用的内存

// 创建新学生节点
Student* createStudent() { // 分配内存给新学生节点
    Student* newStudent = (Student*)malloc(sizeof(Student)); 
    if (newStudent == NULL) { // 检查内存分配是否成功
        printf("内存分配失败！\n");
        exit(1); // 失败则退出程序
    }

    printf("请输入学号: ");
    scanf("%d", &newStudent->id);
    printf("请输入姓名: ");
    scanf("%s", newStudent->name);
    printf("请输入成绩: ");
    scanf("%f", &newStudent->score);

    newStudent->next = NULL;// 新节点暂时不指向任何节点
    return newStudent;// 返回新节点的指针
}

// 添加学生信息
void addStudent() {
    Student* newStudent = createStudent(); // 创建新学生节点

    // 如果链表为空，新节点成为头节点
    if (head == NULL) {
        head = newStudent;
    }
    else {// 找到链表的最后一个节点
        // 否则添加到链表末尾
        Student* current = head;
        while (current->next != NULL) {
            current = current->next;
        } // 最后一个节点的next指向新节点，完成添加
        current->next = newStudent;
    }

    printf("学生信息添加成功！\n");
}

// 显示所有学生信息
void displayAll() {
    if (head == NULL) {// 检查链表是否为空
        printf("链表为空，没有学生信息！\n");
        return;   // 打印表头
    }

    printf("\n===== 学生信息列表 =====\n");
    printf("学号\t姓名\t\t成绩\n");
    printf("------------------------\n");
    //遍历链表并打印每个学生的信息
    Student* current = head;
    while (current != NULL) {
        printf("%d\t%s\t\t%.2f\n", current->id, current->name, current->score);
        current = current->next;// 移动到下一个节点
    }
    printf("------------------------\n");
}

// 搜索学生信息,遍历链表并打印每个学生的信息
void searchStudent() {
    if (head == NULL) {// 检查链表是否为空
        printf("链表为空，没有学生信息！\n");
        return;
    }

    int id;
    printf("请输入要搜索的学生学号: ");
    scanf("%d", &id);
    // 遍历链表查找匹配学号的学生
    Student* current = head;
    while (current != NULL) {
        if (current->id == id) {// 找到匹配的学生
            printf("\n找到学生信息：\n");
            printf("学号: %d\n", current->id);
            printf("姓名: %s\n", current->name);
            printf("成绩: %.2f\n", current->score);
            return; // 找到后直接返回
        }
        current = current->next;// 移动到下一个节点
    }

    printf("未找到学号为 %d 的学生信息！\n", id); // 遍历完未找到
}

// 删除学生信息
void deleteStudent() {
    if (head == NULL) {// 检查链表是否为空
        printf("链表为空，没有学生信息！\n");
        return;
    }

    int id;
    printf("请输入要删除的学生学号: ");
    scanf("%d", &id);

    Student* current = head; // 当前节点
    Student* previous = NULL;// 当前节点的前一个节点

    // 查找要删除的节点 （current指向目标节点，previous指向其前一个节点）
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }

    // 如果未找到
    if (current == NULL) { // 未找到目标节点
        printf("未找到学号为 %d 的学生信息！\n", id);
        return;
    }
    // 删除节点：分两种情况（删除头节点 或 删除中间/末尾节点）
    // 如果要删除的是头节点
    if (previous == NULL) {// 删除头节点
        head = current->next;
    }
    else {// 删除中间或末尾节点
        previous->next = current->next;
    }

    // 释放内存
    free(current);// 释放被删除节点的内存
    printf("学号为 %d 的学生信息已删除！\n", id);
}

// 保存数据到文件
void saveToFile() { // 以二进制写模式打开文件
    FILE* file = fopen(filename, "wb");
    if (file == NULL) { // 打开失败
        printf("无法打开文件进行写入！\n");
        return;
    }
    // 遍历链表，将每个学生的信息写入文件
    Student* current = head;
    while (current != NULL) {
        // 写入学生信息到文件 ,写入时忽略next指针（只存数据，不存指针）
        fwrite(current, sizeof(Student) - sizeof(Student*), 1, file);
        current = current->next;
    }

    fclose(file);// 关闭文件
    printf("数据已成功保存到 %s 文件！\n", filename);
}
// 从文件加载数据
void loadFromFile() {
    // 先释放当前链表
    freeList();// 先释放当前链表的所有节点
    // 以二进制读模式打开文件
    FILE* file = fopen(filename, "rb");
    if (file == NULL) { // 文件不存在或无法打开
        printf("无法打开文件或文件不存在，将创建新文件！\n");
        return;
    }

    Student* current = NULL;
    Student* last = NULL;// 记录链表最后一个节点

    // 从文件读取学生信息
    while (1) { // 循环读取文件中的学生数据
        Student* newStudent = (Student*)malloc(sizeof(Student));// 分配新节点内存
        if (newStudent == NULL) {
            printf("内存分配失败！\n");
            break;
        }
        // 从文件读取学生数据（忽略next指针）
        // 读取学生信息，fread返回实际读取的项目数
        if (fread(newStudent, sizeof(Student) - sizeof(Student*), 1, file) != 1) {
            free(newStudent); // 读取失败，释放内存
            break;
        }

        newStudent->next = NULL;// 新节点的next暂时为空

        // 构建链表,将新节点添加到链表
        if (head == NULL) {// 链表为空时，新节点作为头节点
            head = newStudent;
        }
        else {// 否则添加到链表末尾
            last->next = newStudent;
        }

        last = newStudent;// 更新最后一个节点
    }

    fclose(file);// 关闭文件
    printf("已从 %s 文件加载 %d 条学生记录！\n", filename, head ? 1 : 0);
} // 打印加载的学生数量（head存在则至少1个，否则0个）
// 释放链表内存
void freeList() {
    Student* current = head;
    while (current != NULL) {
        Student* temp = current;// 暂存当前节点
        current = current->next; // 移动到下一个节点
        free(temp);// 释放当前节点的内存
    }
    head = NULL; // 头指针置空，避免野指针
}

// 显示菜单
void showMenu() {// 打印系统功能菜单
    printf("\n===== 成绩信息管理系统 =====\n");
    printf("1. 添加学生信息\n");
    printf("2. 显示所有成绩\n");
    printf("3. 搜索成绩信息\n");
    printf("4. 删除成绩信息\n");
    printf("5. 保存数据到文件\n");
    printf("6. 从文件加载数据\n");
    printf("7. 退出系统\n");
    printf("请选择操作: ");
}

// 主函数
int main() {
    int choice;

    // 程序开始时尝试从文件加载数据
    loadFromFile(); // 程序启动时先从文件加载数据

    while (1) {// 循环显示菜单，直到用户选择退出
        showMenu();
        scanf("%d", &choice);

        switch (choice) {// 根据用户选择执行对应功能
        case 0:// 退出系统
            // 退出前保存数据
            saveToFile();// 退出前保存数据，调用saveToFile函数
            freeList();// 释放内存
            printf("感谢使用，再见！\n");
            return 0;
        case 1://addStudent函数，实现【录入学生成绩、创建链表节点】
            addStudent();
            break;
        case 2://录用学生成绩、打印所有学生的成绩、学号等信息（调用dispiayAll函数）
            displayAll();
            break;
        case 3://按学号/姓名查找，返回匹配的学生信息（调用searchStudent函数）
            searchStudent();
            break;
        case 4://按条件（如学号）找到节点，从链表删除并释放内存（调用deleteStudent）
            deleteStudent();
            break;
        case 5://把链表数据（学生成绩）写入外部文件（持久化存储）【调用saveToFile函数】
            saveToFile();
            break;
        case 6://读取文件内容，还原成链表（程序启动/需要历史数据时用）【调用loadFromFile】
            loadFromFile();
            break;
        default://输入的数字不在1-6范围内，提示用户重新输入
            printf("无效选择，请重新输入！\n");
        }//注意：这里没有break，因为default本身就是“兜底”，执行后自然结束switch
    }

    return 0;//返回0，同时提前退出函数
}