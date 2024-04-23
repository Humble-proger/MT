import pandas as pd

df = pd.read_excel("parse_table.xlsx")
df = df.drop("N", axis=1)
df.to_csv("parse_table.txt", sep='\t', index=False)