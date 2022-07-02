import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { baseURL } from '../../app/apiURL/baseURL';
import { Observable } from 'rxjs';
import { Usuario } from '../models/usuario';
import { Usuariolog } from '../models/usuariolog';
import { Rec } from '../models/rec';
import { CarpetaPropietario } from '../models/carpeta-propietario'

@Injectable({
  providedIn: 'root'
})
export class UsuarioService {

  constructor(private http: HttpClient) {

  }


  //Aqui se crean usuarios
  postCreateCliente(usuario: Usuario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'CreateUser', usuario, httpOptions)
  }
  //Aqui se actualizan los usuarios
  postUpdateUser(usuario: Usuario): Observable<any> {
    console.log(usuario)
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'UpdateUser', usuario, httpOptions)
  }
  //Aqui se verifica si existe el usuario
  postLogin(usuario: Usuariolog): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'Login', usuario, httpOptions)
  }

  postDeshabilitar(usuario: Usuariolog): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'Dishable', usuario, httpOptions)
  }

  //Aqui se recupera la contrasenia
  postRecuperar(usuario: Rec) {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'RecuperarContrasenia', usuario, httpOptions)
  }

  //Aqui se obtiene el arreglo con los usuarios que aun no se han dado de alta
  postDarAlta(usuario: string) {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'DarAlta', usuario, httpOptions)
  }

  //Aqui se obtiene el arreglo con los usuarios que aun no se han habilitado
  postHabilitar(usuario: string) {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'Habilitar', usuario, httpOptions)
  }

  //Aqui se da de alta
  postEnviarAlta(usuario: Usuario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'sendAlta', usuario, httpOptions)
  }

  //Aqui se da de alta
  postEliminar(usuario: Usuario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'eliminar', usuario, httpOptions)
  }

  //Aqui se da de alta
  postHabilitarusr(usuario: Usuario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'HabilitarUsuario', usuario, httpOptions)
  }

  postAddProperty(carpeta: CarpetaPropietario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'ChangeProperty', carpeta, httpOptions)
  }

  postAddColaborador(carpeta: CarpetaPropietario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'addColaborador', carpeta, httpOptions)
  }

  postSyncronize(usuario: string) {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'Syncronize', usuario, httpOptions)
  }


  postCarpetasUsuarios(usuario: Usuario) {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'archivosUsuario', usuario, httpOptions)
  }



  postCarpetasColaboracion(usuario: Usuario) {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'ArchivosColavoracion', usuario, httpOptions)
  }

  postDeleteFichero(carpeta: CarpetaPropietario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'DeleteFichero', carpeta, httpOptions)
  }

  postUppdateCarp(carpeta: CarpetaPropietario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'UUUpdateCarpet', carpeta, httpOptions)
  }

  postAddFichero(carpeta: CarpetaPropietario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'AddFichero', carpeta, httpOptions)
  }


  postMofifyFile(carpeta: CarpetaPropietario): Observable<any> {
    const httpOptions = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json'
      }),
    };
    return this.http.post<any>(baseURL + 'ModifyFile', carpeta, httpOptions)
  }
}