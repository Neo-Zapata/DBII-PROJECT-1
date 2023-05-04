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
Realiza la búsqueda de un registro en función de su key_id. Pasamos el key_id por la  función hash, y el valor obtenido lo utilizamos en nuestro hash table para poder localizar la posición lógica del posible bucket con el registro que buscamos. Luego, una vez encontrado el bucket se procede a verificar de manera secuencial cada uno de los registros hasta encontrarlo, caso contrario se buscará en los registros encadenados hasta  que el bucket no tenga ningún encadenamiento.

### AVL
El árbol AVL es un tipo de árbol binario de búsqueda equilibrado en el que la altura de los subárboles izquierdo y derecho de cada nodo difiere en no más de uno. Esta propiedad permite que al aplicarlo como técnica de organización de archivos las operaciones de inserción, eliminación y búsqueda se realicen en un tiempo de ejecución O(log n). De esta manera, el AVL se situa como una técnica muy efectiva para la gestión de grandes cantidades de registros en un archivo, y es especialmente útil cuando se requiere un acceso rápido y eficiente a los datos almacenados.

Una desventaja del AVL es que para utilizar los registros en una estructura AVL es necesario hacer uso de espacios de memoria adicionales en los que se almacena las posiciones físicas de los 'nodos hijo' izquierdo y derecho, la posición del registro, y la altura.

#### Inserción
1. Primero, se verifica si la archivo está vacío (posición del nodo enviado es -1). En tal caso se crea un nuevo nodo que almacene el record que se quiere insertar. El nuevo nodo también debe tener sus demás campos inicializados. Finalmente se escribe el nuevo nodo al archivo AVL.

2. En caso contrario, se compara si el `anime_id` del registro que se quiere insertar es menor o mayor que el `anime_id` del nodo (root) que se pasa en la función. Si el `anime_id` es menor, el registro se inserta en el subárbol izquierdo. Si el `anime_id` es mayor, se inserta en el subárbol derecho. La inserción se realiza de la misma forma que en el paso 1: creando un nuevo nodo y escribiéndolo en el archivo AVL.

3. Con la función balance, se comprueba si el subárbol resultante después de la inserción sigue estando balanceando. En caso contrario, se aplica rotación de izquierda o derecha, según corresponda, para balancear el AVL. Al finalizar el algoritmo, se tendría el root actualizado para una próxima inserción.

#### Eliminación
1. Primero se verifica si el archivo está vacío, en tal caso, finaliza el algoritmo.

2. Caso contrario, se busca el registro a eliminar. Al encontrar el nodo que corresponde al registro. Se dan los siguientes casos:

  2.1. Si el nodo es una hoja: Se actualiza la posición del nodo a -1 y se realiza un balance.

  2.2. Si el nodo tiene un hijo: Se identifica si el nodo hijo es el izquierdo o el derecho, para intercambiar el hijo en la posición del nodo. Posteriormente se realiza un balance.

  2.3. Si el nodo tiene dos hijos: Se busca el nodo más grande en el subárbol izquierdo del nodo a eliminar y se intercambia lugar con el nodo a eliminar. En esa posición intercambiada se elimina el registro como en 2.1 o 2.2.

#### Búsqueda
1. Primero se verifica si el archivo está vacío, en tal caso, finaliza el algoritmo.

2. En caso contrario, se compara si el `anime_id` del registro es igual al `anime_id` del nodo (root) que se envía a la función. Si son iguales, se ha encontrado el registro buscado, por lo que se retorna el nodo actual.

3. Si el `anime_id` del registro a buscar es menor que el `anime_id` del nodo actual, se busca por el  subárbol izquierdo. En caso, el `anime_id` del registro a buscar es mayor que el `anime_id` del nodo actual se busca por el subárbol derecho.

4. Se repiten los pasos 2 y 3 hasta encontrar el registro buscado o hasta llegar a una hoja del AVL (es decir, un nodo cuyos campos `left` y `right` sean iguales a -1). Si se ha llegado a una hoja del árbol y no se ha encontrado el registro buscado, se retorna un mensaje indicándolo.

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
#### Gráficas y análisis
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
#### Gráficas y análisis
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
#### Gráficas y análisis
##### Inserción
<img src="/images/ext_h_03.png"  width="65%">
El coeficiente de correlación de pearson ajustado para una tendencia lineal es de 0.953, lo cual apunta a que el tiempo de inserción de records es lineal respecto al número de records que se busca insertar.

##### Búsqueda
<img src="/images/ext_h_04.png"  width="65%">
El coeficiente de correlación de pearson ajustado para una tendencia lineal es de 0.0.038, lo cual apunta a que el método de inserción de records no es lineal respecto al número de records que se busca insertar. Así mismo se buscó ajustar a tendencias exponenciales, logarítmicas y polinómicas, sin éxito, obteniéndose un valor de coeficiente bajo. Aquello era algo esperado, ya que la búsqueda no depende de la cantidad de datos insertados. La búsqueda se apoya de una tabla hash, la cual otorga acceso directo al bucket del key_id del registro buscado. Por lo tanto el tiempo de búsqueda tiene una tendencia constante.

##### Eliminación
<img src="/images/ext_h_05.png"  width="65%">
El método de eliminación es similar al método de inserción, ya que internamente tambien hace una búsqueda. Trabaja sobre una tabla hash, la cual otorga acceso directo al bucket del key_id del registro buscado. Y al eliminar solo cambia la posición de dos registros y actualiza el size del bucket. Por lo tanto el tiempo de eliminación tiene una tendencia constante.

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
#### Gráficas y análisis
##### Inserción
<img src="/images/ext_h_06.png"  width="65%">
El coeficiente de correlación de pearson ajustado para una tendencia lineal es de 0.999, lo cual apunta a que el número de accesos a memoria en la inserción de records es lineal respecto al número de records que se busca insertar.

##### Búsqueda
<img src="/images/ext_h_07.png"  width="65%">
El número de accesos de memoria en la búsqueda de un record es constante respecto a la cantidad de registros que se tenga. La tabla_hash interna que se maneja nos permite dar directamente con el bucket del key_id del registro que buscamos. Se recorre una cantidad k de buckets encadenados de no estar en el bucket principal. Siendo ese k el número de accesos a memoria, el cual se repetirá siempre que se busque el mismo record independientemente de la cantidad de records en la base de datos.

##### Eliminación
<img src="/images/ext_h_08.png"  width="65%">
El delete maneja internamente el search para encontrar el record a eliminar y una secuencia de instrucciones adicional que no requiere adicionales accesos a memoria, por lo tanto, el número de accesos de memoria en la eliminación de un record es constante respecto a la cantidad de registros que se tenga.

### AVL
#### Tabla de tiempos (s)
| Iteration | Add   | Search(Zipang) | Remove(Zipang) |
|-----------|-------|----------------|----------------|
| 500	|0,3517|	0,00036|	0,00052|
|1.000|	1,1778|	0,00034|	0,00055|
|1.500|	2,5496|	0,00038| 0,00056|
|2.000|	4,4233|	0,00040|	0,00057|
|2.500|	6,8052|	0,00042|	0,00058|
|3.000|	9,6261|	0,00044|	0,00057|
|3.500|	13,0061|	0,00042|	0,00056|
|4.000	|15,1945|	0,00045|	0,00057|
|4.500	|18,8931|	0,00043|	0,00058|
|5.000	|23,7170|	0,00041|	0,00057|

#### Accesos a disco
| Accesos | Add   | Search(Zipang) | Remove(Zipang) |
|-----------|-------|----------------|----------------|
|500|	126.244|	20|	43|
|1000|	502.494|	20|	43|
|1500|	1.128.744|	20|	43|
|2000|	2.004.994|	20|	43|
|2500|	3.131.244|	20|	43|
|3000|	4.507.494|	20|	43|
|3500|	6.133.744|	20|	44|
|4000|	8.009.994|	20|	44|
|4500|	10.136.244|	20|	44|
|5000|	12.512.494|	20|	44|

## Pruebas de uso y presentacion
Adjuntar fotos, videos, etc. Mostrando la funcionalidad del programa
