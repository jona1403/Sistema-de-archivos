import { Component, OnInit } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
import { usuarioLogueado } from 'src/app/apiURL/baseURL';
import { Usuariolog } from 'src/app/models/usuariolog';
import { UsuarioService } from 'src/app/services/usuario.service';
@Component({
  selector: 'app-actualizar-cuenta',
  templateUrl: './actualizar-cuenta.component.html',
  styleUrls: ['./actualizar-cuenta.component.css']
})
export class ActualizarCuentaComponent implements OnInit {

  hide = true;
  hide2 = true;
  Nombre = new FormControl('');
  Apellido = new FormControl('');
  Usuario = new FormControl('');
  Correo = new FormControl('', [Validators.required, Validators.email]);
  Telefono = new FormControl('');
  contrasenia = new FormControl('');
  mostrarMensaje = false;
  mostrarMensajeError = false;
  constructor(private usuarioService: UsuarioService) { }

  ngOnInit(): void {
    this.Nombre.setValue(usuarioLogueado.nombre)
    this.Apellido.setValue(usuarioLogueado.apellido)
    this.Usuario.setValue(usuarioLogueado.usuario)
    this.Correo.setValue(usuarioLogueado.correo)
    this.Telefono.setValue(String(usuarioLogueado.numero))
    this.contrasenia.setValue(usuarioLogueado.contrasenia)
  }

  validacionCont(): boolean {
    const regex = new RegExp('(?=.*[A-Z])(?=.*[0-9])(?=.*[!"#$%&/()])([A-Za-z0-9!"#$%&/()]){8,20}');

    if (regex.test(this.contrasenia.value as string)) {
      return true;
    }

    return false;
  }


  updateUser() {
    if (!this.validacionCont()) {
      this.mostrarMensajeError = true;
      return;
    }


    usuarioLogueado.nombre = this.Nombre.value as string;
    usuarioLogueado.apellido  = this.Apellido.value as string;
    usuarioLogueado.usuario = this.Usuario.value as string;
    usuarioLogueado.correo = this.Correo.value as string;
    usuarioLogueado.numero = Number(this.Telefono.value);
    usuarioLogueado.contrasenia = this .contrasenia.value as string;

    this.usuarioService.postUpdateUser(usuarioLogueado).subscribe((res: any) => {
      this.mostrarMensaje = true
      console.log("Actualizado")

    }, (err) => {
      this.mostrarMensajeError = true
    })
  }

  desactivarMensaje() {
    this.mostrarMensaje = false
    this.mostrarMensajeError = false
  }

  getErrorMessage() {
    if (this.Correo.hasError('required')) {
      return 'You must enter a value';
    }

    return this.Correo.hasError('email') ? 'Not a valid email' : '';
  }

}
