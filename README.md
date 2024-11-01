# HTTP client to `34.246.184.49`



## Protocolul HTTP
---


![img](img_01_http_request_response.png)

Un protocol aflat la Nivelul 5 din `stiva OSI`,
`HTTP` (**HyperText Transfer Protocol**) joaca un rol important
in viata noastra de zi cu zi, fiind standardul care permite comunicarea si transferul de date pe internet.



Protocolul HTTP se bazeaza pe un o cerere din partea clientului (`HTTP request`),
urmat de un raspuns din partea serverului (`HTTP response`).


### Metode HTTP
---


Clientul poate comunica prin 9 metode:
`GET`, `POST`, `DELETE`, `PUT`, `PATCH`, `HEAD`, `OPTIONS`, `TRACE` si `CONNECT`.


> Clientul implementat de mine se bazeaza doar pe primele trei,
> care sunt **cele mai des folosite** si anume: `GET`, `POST` si `DELETE`.


Intr-o conexiune HTTP, clientul foloseste una dintre aceste metode
pentru a-i spune server-ului ce vrea de la el.


### Coduri HTTP
---

Serverul ii trimite clientului un raspuns,
care contine o **valoare de retur**,
un cod numeric care atesta daca cererea HTTP s-a executat cu sucess sau daca undeva exista erori.


Conform <https://developer.mozilla.org/en-US/docs/Web/HTTP/Status>:
- Informational responses (`100 – 199`)
- Successful responses (`200 – 299`)
- Redirection messages (`300 – 399`)
- Client error responses (`400 – 499`)
- Server error responses (`500 – 599`)


### Token



Dupa fiecare conexiune `HTTP request` - `HTTP response`,
server-ul sterge din memorie clientul (cu alte cuvinte *"uita ca este conectat la el"*),
iar pentru a nu transmite la de fiecare data informatii sensibile,
precum username-ul si parola clientului,
HTTP foloseste un mecanism de autentificare numit `token`.

> Clientul implementat de mine foloseste `JWT` (JSON Web Token).


Cand un utilizator se autentifica (`login`), de exemplu cu username si paroala,
serverul genereaza un `token` (o secventa de caractere, unica pentru fiecare client),
pe care il trmite clientului in `HTTP response`-ul de dupa `logare`.
Clientul va folosi acest `token` in toate urmatoarele cereri HTTP,
pentru a dovedi ca este autentificat
si pentru a nu mai trimite parola si username-ul de fiecare data.


Un `token`, cum este `JWT` (folosit in implementarea clientului meu),
permite o comunicare *"stateless"*, ceea ce inseamna ca
serverul nu trebuie sa stocheze informatii despre utilizatori intre cereri.





## Lucrul cu `POSTMAN`
---

Inainte de a discuta implementarea efectiva a algoritmului pentru `client`,
[`Postman`](https://www.postman.com/) este o aplicatie ce permite interactiunea cu serverul in mod neprogramatic.


> Folosirea `POSTMAN`: https://youtu.be/qyYAOty_bDs

Pentru a afla ce date trebuie sa trimit la server,
am obtinut urmatorul format (raspunsul serverului practic).

### 1. register (`POST`)


INPUT-ul meu:

`POST` 34.246.184.49:8080/api/v1/tema/auth/register


Params -> Body -> JSON
```json
{
    "username": "vesel",
    "password": "wsl"
}
```

> PS: retinem aceste credentiale



OUTPUT-ul de la `POSTMAN` (iconita `</>` din coltul dreapta sus)

```
POST /api/v1/tema/auth/register HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/json
Cookie: Cookie_1=value; connect.sid=s%3Abj8GjL56NundADAZNib_v4ciZdq7q2ve.PbVGUz%2FqmUd4Sx6UKLMvoBDyb5P9tNA7lQg8wgyjZ9Q
Content-Length: 50

{
    "username": "vesel",
    "password": "wsl"
}
```


Jos, cu verde, apara `201 Created`



### 2. login (`POST`)


INPUT-ul mea:

`POST` 34.246.184.49:8080/api/v1/tema/auth/login


Params -> Body -> JSON
```json
{
    "username": "vesel",
    "password": "wsl"
}
```


OUTPU-ul `POSTMAN` (iconita `</>` din coltul dreapta sus)

```
POST /api/v1/tema/auth/login HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/json
Cookie: Cookie_1=value; connect.sid=s%3Abj8GjL56NundADAZNib_v4ciZdq7q2ve.PbVGUz%2FqmUd4Sx6UKLMvoBDyb5P9tNA7lQg8wgyjZ9Q
Content-Length: 50

{
    "username": "vesel",
    "password": "wsl"
}
```


> Daca logarea a fost facuta cu success, vom primi un cookie
> Cookie-ul va fi `connect.sid=s%3AKnXqtRkqHLvRlFdhJZIgC8BSQkp4z6nx.q72T5ptizYZu5jtaQNrAx8TbNFt1PlbAjuPfKguOkC4`.



### 3. enter_library (`GET`)


INPUT-ul meu:

`GET` 34.246.184.49:8080/api/v1/tema/library/access


Mesajul trimis efectiv catre server:

```
GET /api/v1/tema/library/access HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/json
Cookie: Cookie_1=value; connect.sid=s%3Abj8GjL56NundADAZNib_v4ciZdq7q2ve.PbVGUz%2FqmUd4Sx6UKLMvoBDyb5P9tNA7lQg8wgyjZ9Q
Content-Length: 50

{
    "username": "vesel",
    "password": "wsl"
}
```


OUTPUL-ul `POSTMAN` (raspunsul server-ului, 
de data asta se observa un text in partea din jos):
```
{
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX2lkIjoxMzk4OCwiaWF0IjoxNzE2NDc3MjYyLCJleHAiOjE3MTY0ODA4NjIsImF1ZCI6InN0dWRlbnRpIiwiaXNzIjoiUHJvdG9jb2FsZSBkZSBDb211bmljYXRpZSIsInN1YiI6IlRva2VuIHBlbnRydSB0ZW1hIGRlIHByb3RvY29hbGUifQ.jAXHph7hY64CmbsxznfMil-AUlNkwdwdtWiAEOm06B8"
}
```


### Restul continua in aceasi idee (ele se for folosi de `token`-ul de mai sus)


## Implementarea `clientului`
---

Scopul programatorului unui client web este de a afla
ce mesaj sa trimita serverului si formatul in care serverul va raspunde,
aceasta reprezentand ideea principala a temei.

Comenzile clientului vor fi citite de la tastara,
intr-o bucla infinita care se va opri la comanda `exit`.

Fiecare comanda efectueaza cate o cerere `HTTP`
(`POST`, `GET` sau `DELETE`),
trimitand un mesaj cu un format specific cererii
si asteptand raspunsul serverului.


Practic, pentru fiecare comanda:
- citesc datele de la tastatura (acolo unde este cazul)
- verific inputul din consola
- deschid un port de conexiune WEB
- construiesc un mesaj in functie de tipul de cerere web si de informatiile primite de la client
- trimit mesajul construit pe port
- astept raspunsul (practic un sir de caractere) de la server
- aflu codul din raspunsul primit de le `server`
- inchid portul de conexiune



Acesta este sablonul comenzilor din enunt,
pe care le implemtez, folosindu-ma de codul scris in cadrul laboratorului 9 de HTTP.


Ce este in plus fata de acesta este cererea `DELETE`,
care seamana foarte mult cu cea `GET` (doar am schimbat numele cererii `:)`)
In plus, de vreme ce eu lucrez cu un singur `cookie` si un singur `token`,
nu a mai fost nevoie de matrici de string-uri in aceste functii.


In enunt se mentioneaza ca inputul primit de la tastatura sa fie in format JSON
si avand in vedere simplitatea acestuia,
nu am avut nevoie de o biblioteca suplimentara,
ci pur si simplu am respectat formatul JSON:
- un obiect este scris intre acolade `{}`
- numele proprieatatii unui obiect este trecuta intre ghilimele `" "`
- dupa numele proprietatii va urma caracterul doua puncte `:`
- fiecare proprietate va avea un atribut, care va fi trecut intre ghilimele daca acesta nu este string (ex.: id-ul, care este un numar)
- proprieatile unui obiect sunt despartite prin virgula `,`

String-urile in format JSON vor fi corpul (body-ul) efectiv al mesajului catre pagina WEB.




Raspunsul paginii WEB poate fi si acesta in format JSON,
dar nu trebuie deserializat
(motiv pentru care am si decis sa nu complic cu o biblioteca de parsere `JSON`).

Ce este relevant de spus despre raspunsul paginii WEB este ca va intoarce
un cod de retur, un numar intreg cu o semnificatie anume.

Conform <https://developer.mozilla.org/en-US/docs/Web/HTTP/Status>:
- Informational responses (`100 – 199`)
- Successful responses (`200 – 299`)
- Redirection messages (`300 – 399`)
- Client error responses (`400 – 499`)
- Server error responses (`500 – 599`)

Deci, daca intalnim unul din codurile `200 OK` sau `201 Created`
specifica exuctia cu succes a comenzi, drept pentru care
voi afisa la tastatura `SUCCESS`, alaturi de un mesaj sugestiv.

In orice alt caz, fie ca datele de la utilizator sunt invalide,
fie ca token-ul sau cookie-ul nu a fost setat, fie ca
respunsul serverului a fost diferit de `200` sau `201`,
voi afisa `ERROR` cu un text descriptiv.


Pentru comenzile `login` si `enter_library`,
raspunsul returnat de server are o semnificatie mai importanta:
- pentru `login`, vom primi un `cookie` pe baza caruia demonstram serverului ca suntem conectati
- pentru `enter_library`, vom primi un `token` care atesta ca biblioteca a fost accesata dupa logare


Atat `cookie`-ul si `token`-ul sunt niste siruri de caractere
din raspunsul serverului, valorile pe care le voi memora
in doua variabile globale care poarta aceste nume.


Executia comenzilor `enter_library` si `logout`
vor afisa o eroare daca `cookie`-ul nu este setat.

Executia comenzilor `get_books`, `get_book`, `add_book`, `delete_book`
vor afisa si ele un text de eroare daca `cookie`-ul sau `token`-ul nu sunt setate.


La `logout`, vor disparea valorile continute de variabilele globale pentru `cookie` si `token`,
iar pentru a folosi cu `SUCCESS` comenzile de mai sus, cineva trebuie sa se conecteze
si sa isi acceseze biblioteca.


La intalnirea comenzii `exit`, bucla infinita de citire se va opri,
terminand executia progamului
