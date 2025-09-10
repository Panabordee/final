#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct {
    char code[20], name[100], teacher[100];
    int hours;
} Course;

void getCSVPath(char *path, size_t size) {
    char exePath[250];
    if (GetModuleFileName(NULL, exePath, (DWORD)size)) {
        char *lastBackslash = strrchr(exePath, '\\');
        if (lastBackslash) *lastBackslash = '\0';
        snprintf(path, size, "%s\\courses.csv", exePath);
    } else {
        printf("Error getting executable path\n");
        path[0] = '\0';
    }
}

int load(Course cs[]) {
    char path[250];
    getCSVPath(path, sizeof(path));

    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("ไม่สามารถเปิดไฟล์ CSV ได้: %s\n", path);
        return 0;
    }

    char line[256];
 
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return 0;
    }

    int n = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%99[^,],%19[^,],%d,%99[^\n]",
                   cs[n].name, cs[n].code, &cs[n].hours, cs[n].teacher) == 4) {
            n++;
        }
    }

    fclose(fp);
    return n;
}

void save(Course cs[], int n) {
    char path[250];
    getCSVPath(path, sizeof(path));

    FILE *fp = fopen(path, "w");
    if (!fp) {
        printf("Error saving file\n");
        return;
    }

    fprintf(fp, "CourseName,CourseCode,Hours,Instructor\n");
    for (int i = 0; i < n; i++)
        fprintf(fp, "%s,%s,%d,%s\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);

    fclose(fp);
}

void showAll(Course cs[], int n) {
    printf("\n+--------------------------------+----------+-------+--------------------+\n");
    printf("| %-30s | %-8s | %-5s | %-18s |\n", "Course Name", "Code", "Hours", "Instructor");
    printf("+--------------------------------+----------+-------+--------------------+\n");
    for (int i = 0; i < n; i++)
        printf("| %-30s | %-8s | %-5d | %-18s |\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);
    printf("+--------------------------------+----------+-------+--------------------+\n");
}

void add(Course cs[], int *n) {
    Course c;
    printf("ชื่อวิชา: ");
    scanf(" %[^\n]", c.name);
    for (int i = 0; i < *n; i++)
        if (!strcmp(cs[i].name, c.name))
            return printf("ชื่อซ้ำ\n"), (void)0;

    printf("รหัสวิชา: ");
    scanf("%s", c.code);
    for (int i = 0; i < *n; i++)
        if (!strcmp(cs[i].code, c.code))
            return printf("รหัสซ้ำ\n"), (void)0;

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

    printf("\n+--------------------------------+----------+-------+--------------------+\n");
    printf("| %-30s | %-8s | %-5s | %-18s |\n", "Course Name", "Code", "Hours", "Instructor");
    printf("+--------------------------------+----------+-------+--------------------+\n");

    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strstr(cs[i].name, key) || strstr(cs[i].code, key)) {
            printf("| %-30s | %-8s | %-5d | %-18s |\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);
            found = 1;
        }
    }
    if (!found) printf("| %-68s |\n", "ไม่พบข้อมูล");
    printf("+--------------------------------+----------+-------+--------------------+\n");
}

void update(Course cs[], int n) {
    char code[20];
    printf("รหัสวิชาที่จะแก้ไข: ");
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

    printf("ไม่พบรหัสวิชานี้\n");
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

    printf("ไม่พบรหัสวิชานี้\n");
}

int main() {
    Course cs[100];
    int n = load(cs), ch = -1;

    do {
        printf("\n================== COURSE MANAGEMENT ==================\n");
        printf("| 1. ดูทั้งหมด                                    |\n");
        printf("| 2. เพิ่มวิชา                                    |\n");
        printf("| 3. ค้นหา                                       |\n");
        printf("| 4. แก้ไข                                       |\n");
        printf("| 5. ลบ                                          |\n");
        printf("| 0. ออก                                         |\n");
        printf("=======================================================\n");
        printf("เลือก: ");

        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            printf("กรุณาใส่เลข 0-5\n");
            ch = -1;
            continue;
        }

        switch (ch) {
            case 1: showAll(cs, n); break;
            case 2: add(cs, &n); break;
            case 3: search(cs, n); break;
            case 4: update(cs, n); break;
            case 5: del(cs, &n); break;
            case 0: break;
            default: printf("กรุณาใส่เลข 0-5\n");
        }
    } while (ch != 0);

    return 0;
}
