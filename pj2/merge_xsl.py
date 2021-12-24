import os
import pandas as pd
cwd = os.path.abspath('') 
files = os.listdir(cwd) 
df_total = pd.DataFrame()

excel_file = pd.ExcelFile('C:\\Users\\31324\\Desktop\\data_structure\\pj2\\Timetable.xlsx')
sheets = excel_file.sheet_names
for sheet in sheets: # loop through sheets inside an Excel file
    df = excel_file.parse(sheet_name = sheet)
    df_total = df_total.append(df)
df_total.to_excel('combined_file.xlsx')

# data = pd.DataFrame(pd.read_excel('C:\\Users\\31324\\Desktop\\data_structure\\combined_file.xlsx'))#读取数据,设置None可以生成一个字典，字典中的key值即为sheet名字，此时不用使用DataFram，会报错
# print(data.index)#获取行的索引名称
# print(data.columns)#获取列的索引名称
# print(data['line'])#获取列名为姓名这一列的内容
# print(data.loc[0])#获取行名为0这一行的内容
