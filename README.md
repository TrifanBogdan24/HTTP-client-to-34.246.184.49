# PCOM Tema 4
> Enunt: https://pcom.pages.upb.ro/enunt-tema4/about.html
>
> Labortatul 9 de HTTP: https://pcom.pages.upb.ro/labs/lab9/http.html


https://youtu.be/qyYAOty_bDs



## 1. Register (`POST`)

`POST` 34.246.184.49:8080/api/v1/tema/auth/register



```
POST /api/v1/tema/auth/register HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/x-www-form-urlencoded
Cookie: Cookie_1=value; connect.sid=s%3AKnXqtRkqHLvRlFdhJZIgC8BSQkp4z6nx.q72T5ptizYZu5jtaQNrAx8TbNFt1PlbAjuPfKguOkC4
Content-Length: 27

username=sup&password=test1
```


## 2. Login (`POST`)


`POST` 34.246.184.49:8080/api/v1/tema/auth/login




```
POST /api/v1/tema/auth/login HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/json
Cookie: Cookie_1=value; connect.sid=s%3AKnXqtRkqHLvRlFdhJZIgC8BSQkp4z6nx.q72T5ptizYZu5jtaQNrAx8TbNFt1PlbAjuPfKguOkC4
Content-Length: 46

{
    "username": "",
    "password": "juve"
}
```


Cookie-ul va fi `connect.sid=s%3AKnXqtRkqHLvRlFdhJZIgC8BSQkp4z6nx.q72T5ptizYZu5jtaQNrAx8TbNFt1PlbAjuPfKguOkC4`.



## 3. Access (`GET`)


`GET` 34.246.184.49:8080/api/v1/tema/library/access

```
GET /api/v1/tema/library/access HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/json
Cookie: Cookie_1=value; connect.sid=s%3AKnXqtRkqHLvRlFdhJZIgC8BSQkp4z6nx.q72T5ptizYZu5jtaQNrAx8TbNFt1PlbAjuPfKguOkC4
Content-Length: 46

{
    "username": "",
    "password": "juve"
}
```