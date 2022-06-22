import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';

@Component({
  selector: 'app-crear-usuario',
  templateUrl: './crear-usuario.component.html',
  styleUrls: ['./crear-usuario.component.css']
})
export class CrearUsuarioComponent implements OnInit {

  Nombre = new FormControl('');
  Apellido = new FormControl('');
  Usuario = new FormControl('');
  Correo = new FormControl('');
  Telefono = new FormControl('');
  FechaNac = new FormControl('');
  FechaCre = new FormControl('');
  mostrarMensaje = false;
  mostrarMensajeError = false;

  constructor() { }

  ngOnInit(): void {
  }

}
