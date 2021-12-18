import pandas as pd
# 2.把Excel文件中的数据读入pandas
# df = pd.read_excel('Timetable.xlsx')
# print(df)

# # 3.读取excel的某一个sheet
# df = pd.read_excel('Timetable.xlsx', sheet_name='Line 2')
# print(df)

# # 4.获取列标题
# print(df.columns)
# # 5.获取列行标题
# print(df.index)
# # 6.制定打印某一列
# print(df['站名'])

for i in range (1,14):
    df = pd.read_excel('Timetable.xlsx', sheet_name='Line '+str(i))
    print(df)
    df.to_csv('line'+str(i)+'.csv')
df = pd.read_excel('Timetable.xlsx', sheet_name='Line 16')
print(df)
df.to_csv('line16.csv')



