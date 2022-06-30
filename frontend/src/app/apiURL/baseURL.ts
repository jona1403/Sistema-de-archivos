import { Usuario } from "../models/usuario";

export const baseURL = 'http://localhost:3000/';
export var logedin = false;
export var usuarioLogueado: Usuario = new Usuario("","","","",-1,"","","");
export const usuarioAdmin: Usuario = new Usuario('Jonathan','Alvarado', 'ADMIN','jonathanadm715@gmail.com',33820007, '201903004JonathanAlvarado','',''); usuarioAdmin.able = true; usuarioAdmin.alta = true;
