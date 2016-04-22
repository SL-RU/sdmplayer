vals = [10, 11, 12, 13, 15,  16, 18, 20, 22, 24, 27, 30, 33, 34, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91]
v1 = [v * 10 for v in vals]

v1.append(91)
ne = 1000000000
print(v1)
for x in vals:
	for y in v1:
			v = 0.6*(1 + y/x)
			#print(str(v) + " r1 " + str(x) + " r2 " + str(y))
			if ne - 3.3 > v - 3.3 and v > 3.25:
					print("OOOO" + str(v) + " r1 " + str(x) + " r2 " + str(y))
					ne = v