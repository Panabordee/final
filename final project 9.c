#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct {
    char code[20], name[100], teacher[100];
    int hours;
} Course;


void getBasePath(char *path, size_t size) {
    if (GetModuleFileName(NULL, path, (DWORD)size)) {
        char *lastBackslash = strrchr(path, '\\');
        if (lastBackslash != NULL) {
            *lastBackslash = '\0';  
        }
    } else {
        printf("Error getting executable path\n");
        path[0] = '\0'; 
    }
}

int load(Course cs[]) {
    char path[250];
    getBasePath(path, sizeof(path)); 
    FILE *fp = fopen(strcat(path, "\\courses.csv"), "r");
    if (!fp) return 0;
    int n = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%19[^,],%99[^,],%d,%99[^\n]",
                   cs[n].code, cs[n].name, &cs[n].hours, cs[n].teacher) == 4) {
            n++;
        }
    }
    fclose(fp);
    return n;
}

void save(Course cs[], int n) {
    char path[250];
    getBasePath(path, sizeof(path)); 
    FILE *fp = fopen(strcat(path, "\\courses.csv"), "w");
    if (!fp) {
        printf("Error saving file\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%s,%d,%s\n", cs[i].code, cs[i].name, cs[i].hours, cs[i].teacher);
    }
    fclose(fp);
}

void add(Course cs[], int *n) {
    Course c;
    printf("ชื่อวิชา: ");
    scanf(" %[^\n]", c.name);
    for (int i = 0; i < *n; i++) if (!strcmp(cs[i].name, c.name))
        return printf("ชื่อซ้ำ\n"), (void) 0;

    printf("รหัสวิชา: ");
    scanf("%s", c.code);
    for (int i = 0; i < *n; i++) if (!strcmp(cs[i].code, c.code))
        return printf("รหัสซ้ำ\n"), (void) 0;

    printf("ชั่วโมง: ");
    scanf("%d", &c.hours);
    printf("อาจารย์: ");
    scanf(" %[^\n]", c.teacher);
    cs[(*n)++] = c;
    save(cs, *n);
    printf("เพิ่มแล้ว\n");
}

void search(Course cs[], int n) {
    char key[100];
    printf("ค้นหาด้วยชื่อวิชาหรือรหัส: ");
    scanf(" %[^\n]", key);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strstr(cs[i].name, key) || strstr(cs[i].code, key)) {
            printf("%s (%s), %d ชม., %s\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);
            found = 1;
        }
    }
    if (!found) printf("ไม่พบ\n");
}

void update(Course cs[], int n) {
    char code[20];
    printf("รหัสวิชาที่จะแก้: ");
    scanf("%s", code);
    for (int i = 0; i < n; i++)
        if (!strcmp(cs[i].code, code)) {
            printf("แก้ไข %s (%s)\n", cs[i].name, cs[i].code);
            printf("ชั่วโมงใหม่: ");
            scanf("%d", &cs[i].hours);
            printf("อาจารย์ใหม่: ");
            scanf(" %[^\n]", cs[i].teacher);
            save(cs, n);
            printf("อัพเดทแล้ว\n");
            return;
        }
    printf("ไม่พบ\n");
}

void del(Course cs[], int *n) {
    char code[20];
    printf("รหัสวิชาที่ลบ: ");
    scanf("%s", code);
    for (int i = 0; i < *n; i++)
        if (!strcmp(cs[i].code, code)) {
            printf("ลบ %s (%s)\n", cs[i].name, cs[i].code);
            for (int j = i; j < *n - 1; j++) cs[j] = cs[j + 1];
            (*n)--; 
            save(cs, *n);
            printf("ลบแล้ว\n");
            return;
        }
    printf("ไม่พบ\n");
}

int main() {
    Course cs[100];
    int n = load(cs), ch = -1;
    do {
        printf("\n1.ดูทั้งหมด 2.เพิ่ม 3.ค้นหา 4.แก้ไข 5.ลบ 0.ออก\nเลือก: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            printf("กรุณาใส่เลข 0-5\n");
            ch = -1;
            continue;
        }
        if (ch < 0 || ch > 5) {
            printf("กรุณาใส่เลข 0-5\n");
            continue;
        }

        if (ch == 1)
            for (int i = 0; i < n; i++)
                printf("%s (%s), %d ชม., %s\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);
        else if (ch == 2) add(cs, &n);
        else if (ch == 3) search(cs, n);
        else if (ch == 4) update(cs, n);
        else if (ch == 5) del(cs, &n);
    } while (ch != 0);
}

