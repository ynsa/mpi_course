#pragma omp <directive-name> [options]
omp_<function_name>();
<omp.h>

#pragma omp parallel [options]
{

}
// На выходе из параллельной области - барьерная синхронизация

/* опции
	num_threads(count) - количество потоков (мастер тоже включается)

	export OMP_NUM_THREADS = count(если указать в одной программе оба вариана, первый будет приритетным)


	Две классные ф-ии для получения собственного ранга и количества пооков.

	Если нити ачинают переопределять общие переменные, то появляются специальные секции.

	МОЖЕТ БЫТЬ ЧТО УГОДНО ПРИ ОБЪЯВЛЕНИИ(?????????????) rank

*/

#pragma omp parallel private(rank)
{
	rank = omp_get_thread_num();
}


int rank = 0; //просуммированные значения будут аккумулируются с изначатльным
#pragma omp parallel reduction(+:var){ // +, * и т.д. var - переменные, которые надо ак-ть
	
	#pragma omp single [options]{ //если в опциях nowait, то другие потоки идут дальше, если такой опции нет, то все ждут тот один поток, который выполняет код(барьерная синхронизация)
	
	}

	copyprivate() // результат выполнения single присваивается всем одноименным переменным во всех нитях


	//...
	#pragma omp master // то же самое, что и single, но выполняет строго мастер. БАРЬЕРНОЙ СИНХРОНИЗАЦИИ НЕТ!
}

// Директивы распределения вычислительной нагруж=зки
#pragma omp parallel for [options] // область только для параллельного цикла
for(i = 0; i < n; ++i) // у каждой нити своя копия i, поэтому мы не можем знать, какое значение у i будет на выходе из блока

last_private(i) // тогда будет присвоено значение i из самой последней итерации

// задача рапределения наргрузки

schedue(type [, chunk])

type:
	static  //делит равномерно и никого не обижает - хорошая функция, всё честно.
		, chunk // блочно-циклическое распределение chunk - размер блока (по умолчанию n / p - представляете?!)

	dynamic
		, chunk // каждая нить получает по блоку, а дальше - кто быстрее (тоже достаточно справедливо) Это та самая модель!!! Самая-самая топ-модель мастер-слейв(а это вообще-то запретили питонистам говорить, но, наверное, он не питонист, поэтому ему можно) о-хо-хо

	guided
		, chunk // парам-пам-пам, тут меняется размер. Размер стартового блока предопределён, как и вся наша жизнь(но это не точно), а дальше уменьшает бла-бла-бла до параметра chunk. Маленький блок - очень неэффективно :с


о!
о-о-о-о-о-о

#pragma omp ordered // падказвае, што аперацыі павінны выконвацца ў строгім парадку, які абумоўлен ітэрацыяй(важна, калі існуе залежнасці паміж ітэрацыямі). Прыклад: for{a = f(a)}
