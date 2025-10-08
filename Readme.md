Markdown

<div align="center">

  *
 ***
*****
 ***
  *

# ❄️ CMS - Course Management System ❄️

*โปรเจกต์นี้คือโปรแกรมจัดการรายวิชา (Course Management System) แบบง่ายๆ ทำงานบน Console เขียนด้วยภาษา C*

![Language](https://img.shields.io/badge/Language-C-00599C.svg) ![Version](https://img.shields.io/badge/Version-1.0-blue.svg) ![Platform](https://img.shields.io/badge/Platform-Windows-lightblue.svg)

</div>

---
<br>

## 🚀 Getting Started
ทำตามขั้นตอนเหล่านี้เพื่อเริ่มต้นใช้งานโปรแกรม

### **1. Clone Project**
เปิด Terminal แล้วใช้คำสั่งนี้เพื่อคัดลอกโปรเจกต์ลงมาที่เครื่อง
```bash
git clone [https://github.com/Panabordee/final.git](https://github.com/Panabordee/final.git)
cd final

2. Compile

คอมไพล์โปรแกรมโดยใช้คำสั่ง gcc แบบระบุไฟล์ทั้งหมดด้วยตัวเอง:
Bash

gcc main.c unittests.c e2etests.c -o course_manager.exe

คำสั่งนี้จะสร้างไฟล์โปรแกรมที่ชื่อ course_manager.exe ขึ้นมา

3. Run

หลังจากคอมไพล์เสร็จ สามารถรันโปรแกรมด้วยคำสั่งนี้:
Bash

./course_manager.exe

    สำคัญ: ต้องมีไฟล์ courses.csv อยู่ในโฟลเดอร์เดียวกันกับ course_manager.exe ด้วยนะ!

🧠 Code Logic

ตรรกะของโปรแกรมนี้แบ่งเป็น 2 ส่วนหลักๆ คือโครงสร้างไฟล์และการไหลของข้อมูล

File Structure

เราแยกโค้ดตามหน้าที่ของมันเพื่อให้จัดการง่าย:

    course.h: เป็น "พิมพ์เขียว" หลักของโปรเจกต์ บอกทุกไฟล์ว่ามีฟังก์ชันอะไรให้ใช้บ้าง

    main.c: จัดการ "เมนูหลัก" และเป็นจุดเริ่มต้น

    unittests.c & e2etests.c: ไฟล์เหล่านี้มีหน้าที่ ทดสอบโปรแกรม เท่านั้น

Data Flow

โปรแกรมทำงานกับข้อมูลแบบง่ายๆ คือ:
Diff

+ 1. อ่านทั้งหมด: ตอนเปิดโปรแกรม มันจะอ่านข้อมูลทั้งหมดจาก courses.csv มาเก็บไว้ใน RAM
# 2. แก้ไขในแรม: เวลาเรา เพิ่ม, ลบ, หรือแก้ไขข้อมูล โปรแกรมจะทำกับข้อมูลที่อยู่ใน RAM ก่อน
- 3. เขียนทับทั้งหมด: เมื่อมีการเปลี่ยนแปลงข้อมูล โปรแกรมจะเอาข้อมูลทั้งหมดที่อยู่ใน RAM ไปเขียนทับไฟล์ courses.csv



