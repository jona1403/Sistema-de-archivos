import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { baseURL } from '../../app/apiURL/baseURL';
import { Observable } from 'rxjs';
import { Usuario } from '../models/usuario';
import { Usuariolog } from '../models/usuariolog';
import { Rec } from '../models/rec';

@Injectable({
  providedIn: 'root'
})
export class UsuarioService {

  constructor(private http: HttpClient) {

  }

  postCreateCliente(usuario:Usuario): Observable<any> {
    const httpOptions = {
      headers : new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL+'CreateUser', usuario, httpOptions)
  }

  postLogin(usuario:Usuariolog): Observable<any> {
    const httpOptions = {
      headers : new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL+'Login', usuario, httpOptions)
  }


  postRecuperar(usuario:Rec){
    const httpOptions = {
      headers : new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL+'RecuperarContrasenia', usuario, httpOptions)
  }

  postDarAlta(usuario: string){
    const httpOptions = {
      headers : new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL+'RecuperarContrasenia', usuario, httpOptions)
  }
}
