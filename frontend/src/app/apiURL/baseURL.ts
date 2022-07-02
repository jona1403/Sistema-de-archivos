import { Usuario } from "../models/usuario";
import {archivo, carpeta} from '../componentes/inicio/inicio.component';

export const baseURL = 'http://localhost:3000/';
export var logedin = false;
export var usuarioLogueado: Usuario = new Usuario("","","","",-1,"","","");
export var archivovar:archivo = {grupo:[], propietario:"", nombre:"", texto:"", tipo:-1};
export var carpetavar:carpeta = {grupo:[], propietario:"", nombre:"", contenido:[], tipo:-1};
export const usuarioAdmin: Usuario = new Usuario('Jonathan','Alvarado', 'ADMIN','jonathanadm715@gmail.com',33820007, '201903004JonathanAlvarado','',''); usuarioAdmin.able = true; usuarioAdmin.alta = true;
