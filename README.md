# DBII-PROJECT-1

## Introduccion

### Objetivos del proyecto
Implementar las estructuras de manejo de archivo sequential-file, avl-file y extendible hashing y sus métodos asociados de inserción, búsqueda y eliminación. Así mismo, evaluar la eficiencia de cada uno de los métodos a través de la generación de gráficos que relacionen cantidad de registros con tiempo de ejecución de un método.

### Descripcion del dataset
Se utilizaron datasets pertenecientes a la página https://www.kaggle.com/datasets, los cuáles fueron descargados como csv y cargados en nuevos archivos de datos para el correcto funcionamiento de los algoritmos de manejo de archivos físicos.

### Resultados esperados
Funcionamiento adecuado de las estructuras de manejo de archivos y sus métodos asociados. Comportamiento asintótico experimental semejante al comportamiento asintótico teórico.

## Tecnicas utilizadas

### Sequential File Indexing
Sequential File es una tecnica usada para mejorar el rendimiento en busqueda y acceso de data en el archivo. En un Sequential File los registros se guardan de forma ordenada (sequencial), tipicamente en memoria secundaria.

Sequential File es una simple y efectiva forma de mejorar el rendimiento de obtencion de data (Data retrieval). Sin embargo, tiene limitaciones, como la necesidad de un archivo auxiliar, y el mantenimiento de los punteros `prev` y `next` cuando se eliminan o añaden registros.
#### Insercion 
Los registros nuevos son insertados en un archivo auxiliar. Cada registro cuenta con punteros `next` y `prev` que apuntan al resto de registros en el archivo.

Se realiza una busqueda binaria O(logn) en el `DataFile` (archivo orginal ordenado). El cual retorna una posición (posición física - archivo) cercana a la que se debe insertar. Debido a que puede haber registros en el archivo auxiliar `AuxFile` y la busqueda es solamente en el `DataFile`. Por lo que, a partir de la posición cercana se busca 3 posiciones no necesariamente contiguas (begin, mid, end). Para actualizar los punteros solo se necesitan 3 posiciones, la posicion de insercion (mid), el anterior a este (begin) y el siguiente (end).

![Alt Text](/images/pointers1.png)
#### Eliminacion
Los registros a eliminar se buscan usando busqueda binaria (igual que en la Inserción) y se retoran una posición cercan en el `DataFile`. A partir de esta posicion volvemos a conseguir las 3 posiciones begin, mid y end. Se evaluan los casos (si es el primer registro, el ultimo registro, cualquier registro). Se cambia el atributo `is_deleted = true`. y se cambian los punteros del registro anterior y siguiente. (Cada registro cuenta con punteros `next` y `prev` que apuntan al resto de registros en el archivo.)


![Alt Text](/images/pointers2.png)
#### Busqueda
El registro a buscar puede o no estar en el `DataFile`. Sea el caso que sea, sabemos que el `DataFile` está ordenado y el `AuxFile` no. Entonces, se realiza una busqueda binaria en el `DataFile`, el cual retorna una posicion cercana a la que estamos buscando. En este caso, usamos los métodos `look_before()` y `look_after()` para recorrer los registros usando los punteros `next` y `prev`. Hasta encontrar los registros que cumplan con la busqueda.

![Alt Text](/images/pointers3.png)
### Extendible hashing
El extendible hash es una estructura de almacenamiento dinámico. Consta de 3 elementos importantes, registros, buckets y el hash table. Los registros son un conjunto de información relacionada los cuales se encuentran en un inicio en el archivo dataset y posteriormente en buckets, estructuras de almacenamiento de registros. Así mismo, el hash_table es un conjunto de asociaciones key-value en donde el key es la llave a través del cual se puede llegar al value, valor en específico.  La estructura extendible hash emula el funcionamiento de una base de datos, la organización, accesibilidad y perdurabilidad de los datos. Los archivos index.dat y data.dat me permiten manejar la perdurabilidad de los registros. Dentro de index.dat se almacenan las asociaciones key-value del hash_table mientras que en data.dat buckets de registros.
![Alt Text](/images/ext_h_01.png)

#### Insercion
Se encarga de añadir registros a la base de datos. La inserción de registros contempla la existencia de llaves primarias en los registros y la no repetición de los mismos en la base de datos. En ese sentido, al momento de insertar un registro se hace un llamado a la función search. Luego, una vez visto que el registro tiene llave primaria única, se utiliza la función hash para poder ubicar el registro en un bucket correspondiente, mediante la iteración sobre el resultado de la función hash. Es así que, luego de encontrar el bucket correspondiente al registro que se va insertar se hacen verificaciones antes de insertarla. Como cada bucket tiene un factor de bloque, se verifica si el size de ese bucket es menor a dicho factor de bloque, si es así, se inserta normalmente en el array de registros que contiene el bucket. Si no se realiza la siguiente verificación, la cuál consiste en comparar el índice de profundidad Global (El cuál es el primer parámetro en la construcción de nuestra clase) con el índice de profundidad local que posee cada bucket. Si es menor, se procede crear un nuevo bucket, siempre actualizando los índices, es decir también se crea un nuevo bucket, y se procede a insertar los registros en ambos buckets de acuerdo a la función hash. Caso contrario, en el cuál la profundidad global es igual a la profundidad local, se procede a hacer un encadenamiento de un nuevo bucket que será insertado al final, sin olvidarse de actualizar el next del bucket asociado a este.
![Alt Text](/images/ext_h_02.png)

#### Búsqueda
Se realizan los mismos procedimientos que la búsqueda. Sin embargo, se agrega que luego de encontrarlo se pasa a intercambiar el registro de la posición size-1 del bucket con el de la posición del registro que buscamos. Luego se actualiza el size del bucket en size-=1 y listo.

#### Eliminación
Realiza la búsqueda de un registro en función de su key_id. Pasamos el key_id por la  función hash, y el valor obtenido lo utilizamos en nuestro hash table para poder localizar la posición lógica del posible bucket con el registro que buscamos. Luego, una vez encontrado el bucket se procede a verificar de manera secuencial cada uno de los registros hasta encontrarlo, caso contrario se buscará en los registros encadenados hasta  que el bucket no tenga ningún encadenamiento

### tecnica 3
#### Insercion
#### Eliminacion
#### Busqueda

## Resultados Experimentales
### Sequential (without rebuild - worst case - O(log(k) + h))
#### Tabla de tiempos (s)
| Iteration | Add   | Search(Zipang) | Remove(Zipang) |
|-----------|-------|----------------|----------------|
| 500	|2,2610|	0,01796|	0,01997|
|1.000|	8,0690|	0,03478|	0,03660|
|1.500|	16,6026|	0,04994|	0,04865|
|2.000|	28,8230|	0,06883|	0,06653|
|2.500|	42,3275|	0,08803|	0,08592|
|3.000|	59,7987|	0,09456	|0,10244|
|3.500|	83,3470|	0,11938|	0,11774|
|4.000	|110,6170|	0,12444|	0,13303|
|4.500	|146,6750	|0,15387|0,14543|
|5.000	|178,3900|	0,17524|	0,17883    |
#### Gráficas
![Alt Text](/images/seq_at_wr.png)
![Alt Text](/images/seq_st_wr.png)
![Alt Text](/images/seq_rt_wr.png)

#### Accesos a disco
| Accesos | Add   | Search(Zipang) | Remove(Zipang) |
|-----------|-------|----------------|----------------|
|500|	50.740|	413|	420|
|1000|	205.983|	845|	854|
|1500|	442.499|	1.255|	1.264|
|2000|	774.521|	1.673|	1.682|
|2500|	1.166.880|	2.099|	2.108|
|3000|	1.640.799|	2.511|	2.519|
|3500|	2.309.872|	2.955|	2.964|
|4000|	3.047.413|	3.382|	3.391|
|4500|	3.873.290|	3.797|	3.806|
|5000|	4.754.554|	4.225|	4.234|
### Sequential (with rebuild - best & average case - O(log(n)))
#### Tabla de tiempos (s)
| Iteration | Search(Zipang)   | Remove(Zipang) | Add |
|-----------|-------|----------------|----------------|
| 500	|0,0000690|	0,0014540|	0,52298|
|1.000|	0,0004870|	0,0014340|	1,05269|
|1.500|	0,0004960|	0,0014680|	1,52214|
|2.000|	0,0004040|	0,0019310|	2,05403|
|2.500|	0,0004170|	0,0014600|	2,55277|
|3.000|	0,0004350|	0,0011360|	3,06404|
|3.500|	0,0003680|	0,0013210|	3,99919|
|4.000|	0,0004260|	0,0011300|	4,11042|
|4.500|	0,0005170|	0,0015750|	4,86000|
|5.000|	0,0005260|	0,0018820|	5,25290  |
#### Gráficas
![Alt Text](/images/seq_at_r.png)
![Alt Text](/images/seq_st_r.png)
![Alt Text](/images/seq_rt_r.png)

#### Accesos a disco
| Accesos | Search(Zipang)   |  Remove(Zipang) | Add |
|-----------|-------|----------------|----------------|
|500|	11|	17|	10.990|
|1000|	12|	21|	22.982|
|1500|	12|	21|	35.459|
|2000|	13|	22|	47.959|
|2500|	13|	22|	60.912|
|3000|	14|	22|	73.911|
|3500|	14|	23|	86.916|
|4000|	14|	23|	99.915|
|4500|	14|	23|	113.315|
|5000|	14|	23|	126.815|

### Extendible Hash
#### Tabla de tiempos (s)
| Registros | Inserción | Búsqueda | Eliminación |
|-----------|-------|----------------|----------------|
| 500	|0,1502|0,00038477	|0,00008395	|
|1.000|0,4472|0,00032100	|0,00007445	|
|1.500|0,4110|0,00033504	|0,00007753	|
|2.000|0,6523|0,00029516	|0,00008479	|
|2.500|0,6929|0,00029784	|0,00010769 |
|3.000|0,7814|0,00026897	|0,00008123 |
|3.500|1,1101|0,00033798	|0,00007683	|
|4.000|1,1718|0,00044365	|0,00007669	|
|4.500|1,3454|0,00037787  |0,00010833 |
|5.000|1,2573|0,00034147	|0,00007858	|
#### Gráficas
<img src="/images/ext_h_03.png"  width="70%">
<img src="/images/ext_h_04.png"  width="70%">
<img src="/images/ext_h_05.png"  width="70%">

#### Accesos a disco
| Registros | Inserción | Búsqueda | Eliminación |
|-----------|-------|----------------|----------------|
|500	|3.475|1|2|
|1.000|6.621|1|2|
|1.500|9.417|1|2|
|2.000|12.933|1|2|
|2.500|15.310|1|2|
|3.000|18.164|1|2|
|3.500|22.114|1|2|
|4.000|24.979|1|2|
|4.500|27.526|1|2|
|5.000|30.951|1|2|
#### Gráficas
![Alt Text](/images/ext_h_06.png)

## Pruebas de uso y presentacion
Adjuntar fotos, videos, etc. Mostrando la funcionalidad del programa
