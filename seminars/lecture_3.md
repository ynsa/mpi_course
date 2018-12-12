// ПРОИЗВОДНЫЕ ТИПЫ ДАННЫХ
// является ничем иным, как шаблоном для выборки данных, для передачи или приёма в памяти.

Характеристики производного типа:
	- карта типа:
		```Typemap = {(type_0, disp_0), (type_1, disp_1), ..., (type_n, disp_n)}```
		Для ```MPI_DOUBLE``` имеем:
		```Typemap(MPI_DOUBLE) = {(double, 0)}```
		- нижняя(левая) граница типа:
			```lb(type) = min(disp_i)```
			Минимальное смещение не обязательно должно находиться в первой паре в карте типа.
		- верхняя(правая) граница:
			```rb(type) = max(disp_i + sizeof(type_i))```
			Правая граница не обязана находиться по приведённой выше формуле, она может быть в другом месте(в памяти даже до конца всех данных произвольного типа).
		- ```extent(type) = rb - lb```

Конструкторы производных типов(все они будут переменными типа ```MPI_Datatype```):
1. 
	```
	MPI_Type_contiguous(
		int count, 
		MPI_Datatypr ddtype, 
		MPI_Datatype* newType
	);
	```
	Как строится новый тип:
	```
	oldtype = {(type_0, disp_0), (type_1, disp_1), ..., (type_n, disp_n)}
    newtype = {(type_0, disp_0), (type_1, disp_1), ..., (type_n, disp_n),
   			  (type_0, disp_0 + extend), (type_1, disp_1 + extend), ..., (type_n, disp_n + extend)
			  (type_0, disp_0 + 2 * extend), (type_1, disp_1 + 2 * extend), ..., (type_n, disp_n + 2 * extend),
			  ...
			  (type_0, disp_0 + (count - 1) * extend), (type_1, disp_1 + (count - 1) * extend), ..., (type_n, disp_n + (count - 1) * extend)}
	```
	Пример:

	``` 
		int a[N];
		MPI_Datatype seqtype;
		MPI_Type_contiguous(N, MPI_INT, &seqtype);
		MPI_Type_commit(&seqtype); // обязательное действие при использовании типа при пересылке данных
		MPI_Send(a, 1, seqtype, ...); // 1 - сколько раз используем шаблон
		MPI_Type_free(&seqtype);
	```

	Пример карты типа:
		```
		MPI_Type_contiguous(5, MPI_DOUBLE, &seqtype);
		newtype = {(double, 0), (double, 8), (double, 16), (double, 24), (double, 32)}
		```

	Рассмотрим тип чуть сложнее: 
	``` ddtype = {(char, 0), (int, 1)}``` // всего байт 5, но ```extent``` равен восьми

	При передаче данных выберется 5 байт, соотвтетствующих типам данных нового типа, extent на это не влияет.
	```
		MPI_Type_contiguous(3, ddtype, &seqtype);
		seqtype = {(char, 0), (int, 1),
					(char, 8), (int, 9),
					(char, 16), (int, 17)}
		```
	extent - можно сказать, "выравнивание" (пять выравниваем до восьми)

2. ```MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype ddtype, MPI_Datatype* nexttype);```
	В пределах блока эл-ты расположены послеовательно, всего элементов в блоке ```blocklength```, ```stride``` - расстояние между началами двух сосведних блоков, всего блоков - ```count```. Блок - посл-ть из такого количества эл-ов базового типа, что если всё развернуть, то смещение правого(?) эл-та и будет началом блока.
	Сформируем первый блок из ```count```:
	```
	nexttype = {(type_0, disp_0), (type_1, disp_1), ..., (type_n, disp_n),
   			  (type_0, disp_0 + extend), (type_1, disp_1 + extend), ..., (type_n, disp_n + extend)
			  (type_0, disp_0 + 2 * extend), (type_1, disp_1 + 2 * extend), ..., (type_n, disp_n + 2 * extend),
			  ...
			  (type_0, disp_0 + (bl - 1) * extend), (type_1, disp_1 + (bl - 1) * extend), ..., (type_n, disp_n + (bl - 1) * extend)}}
	```

	Сформируем теперь второй блок. Началом второго блока является ```disp_0```:
	```
	nexttype = {(type_0, disp_0 + str * ex), (type_1, disp_1 + str * ex), ..., (type_n, disp_n + str * ex),
   			  (type_0, disp_0 + str * ex + extend), (type_1, disp_1 + str * ex + extend), ..., (type_n, disp_n + str * ex + extend)
			  (type_0, disp_0 + 2 * extend), (type_1, disp_1 + 2 * extend), ..., (type_n, disp_n + 2 * extend),
				...}
	```

	Рассмотрим пример:

	```
	MPI_Type_contiguous(3, 2, 5, MPI_CHAR, &nexttype);
	nexttype = {(char, 0), (char, 1),
				(char, 5), (char, 6),
				(char, 10), (char, 11)}
	```
	Пример двумерного массива(я так поняла, пример для выбора конкретного столбца):
	```
	int a[n][m];
	MPI_Type_vector(n, 1, m, MPI_INT, &coltype);
	MPI_Send(&a[0][j], 1, coltype, ...);
	// тут ещё был приём в строку
	```

	Теперь выбираем блок размера ```r_1```x```r_2```.
	```
	MPI_Type_vector(r_1, r_2, m, MPI_INT, &blocktype);
	MPI_Send(&a[i][j], 1, blocktype, ...);
	...
	MPI_Type_vector(n, 1, -1, &blocktype); // реверсивная запись массива
	```
3. ```MPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype ddtype, MPI_Datatype* nexttype);```
```MPI_Aint``` - побайтовый параметр(т.е. теперь задаём смещение в байтах).
Пример:
	```
	MPI_Type_hvector(3, 2, 2, MPI_INT, &nexttype);
	```
	Теперь мы хотим транспонировать матрицу при записи. Вектор нам не подходит, т.к. данные у нас не совсем чередующиеся(я не помню).
	Значит, возьмём и создадим тип колонка:
	```
	MPI_Type_vector(n, 1, m, MPI_INT, &coltype);
	MPI_Send(a, m, coltype, ...); //неправильно, т.к. выйдем за пределы масива, т.к. при втором обращении система прыгнет на extent, а это приведёт к прыжку вне матрицы.
	```
	Что же делать?
	```MPI_Type_hvector```!!!
	```
	MPI_Type_hvector(m, 1, sizeof(int), coltype, &transtype);
	MPI_Send(a, 1, transtype,...);
	MPI_Recs(a_recv, n * m, MPI_INT, ... );
	```

4. ```MPI_Type_indexed(int count, int* array_of_blocklength, int* array_of_disp, MPI_Datatype ddtype, MPI_Datatype* nexttype);```
	Теперь размер фрагментов может отличаться и их смещение тоже. Смещения рассчитываются относительно начала базового типа(его disp_0).
	Пример:
	```MPI_Type_indexed(4, {3, 1, 2, 4}, {0, 2, 4, 7}, MPI_INT, &nexttype);```
	В этом примере стоит обратить внимание на то, что послежний элемент первого блока является единственным лементом второго лока, т.е. блоки в памяти огут накладываться друг на друга.

	Пример. Предположим, нам известно, что матрица треугольная, соответственно, можем передать не всю матрицу, а лишь её значимую часть.
	```MPI_Typeindexed(n, {1, 2, ..., n}, {0, n,  2 * n, ..., (n - 1) * n}, MPI_INT, &nexttype);```
	При таком подходе не стоит забывать, что на принимающей стороне матрица должна быть инициализирована нулями.
	Существует модифткация ф-ии ```hindexed```. Заменяем массив смещений ```Aint*```.