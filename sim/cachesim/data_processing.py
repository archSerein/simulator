import sys
import matplotlib.pyplot as plt
import pandas as pd
import re
from io import StringIO

# 模拟从标准输入读取（此处用 StringIO 模拟，实际脚本中可直接使用 sys.stdin.read()）
# with open('log', 'r') as f:
#     input_text = f.read()
input_text = sys.stdin.read()

# 正则提取 block、associativity、hit rate
pattern = r'block: (\d+), associativity: (\d+).*?hit rate: ([0-9.]+)'
matches = re.findall(pattern, input_text)

sorted_data = sorted(matches, key=lambda x: int(x[1]))

# 构建 DataFrame
df = pd.DataFrame(sorted_data, columns=['block', 'associativity', 'hit_rate'])
df = df.astype({'block': int, 'associativity': int, 'hit_rate': float})

# 按 block 分类绘图
blocks = sorted(df['block'].unique())
for block in blocks:
    subset = df[df['block'] == block]
    plt.figure()
    plt.plot(subset['associativity'], subset['hit_rate'], marker='o')
    plt.title(f'Hit Rate vs Associativity (Block Size: {block})')
    plt.xlabel('Associativity')
    plt.ylabel('Hit Rate')
    plt.grid(True)
    plt.xticks(subset['associativity'].unique())
    plt.tight_layout()
    plt.show()
