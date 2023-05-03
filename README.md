# DBII-PROJECT-1

## Introduccion

### Objetivos del proyecto

### Descripcion del dataset
Obtener la informacion de Kaggle: 
- Titanic: ...
- Pokemón: ...

### Resultados esperados

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
### tecnica 2
#### Insercion
#### Eliminacion
#### Busqueda

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
## Pruebas de uso y presentacion
Adjuntar fotos, videos, etc. Mostrando la funcionalidad del programa