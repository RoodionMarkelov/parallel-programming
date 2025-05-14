from scipy.stats import norm
P = 0.85 		  #вероятность обнаружения
Ф_1 = norm.ppf(P)  #вычисление квантиля (обратной функции Лапласа)
print(Ф_1)

