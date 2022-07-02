import { Component, OnInit } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
import { Usuariolog } from 'src/app/models/usuariolog';
import { UsuarioService } from '../../services/usuario.service';
import { Router, UrlSegment, UrlSerializer } from '@angular/router';
import { logedin, usuarioAdmin, usuarioLogueado } from 'src/app/apiURL/baseURL';
@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {
  hide = true;
  correo = new FormControl('', [Validators.required, Validators.email]);
  contrasenia = new FormControl('');
  mostrarMensaje = false;
  mostrarMensajeError = false;
  contador = 0;
  constructor(private usuarioService: UsuarioService, public router: Router) { }

  ngOnInit(): void {
  }

  login() {

    const user: Usuariolog = {
      usr: this.correo.value as string,
      cntr: this.contrasenia.value as string
    }

if(user.usr == '' || user.cntr == ''){
  this.router.navigateByUrl('/denied')
}

    if (user.usr == usuarioAdmin.correo && user.cntr == usuarioAdmin.contrasenia) {
      this.correo.setValue('');
      this.contrasenia.setValue('');
      this.mostrarMensaje = true;
      this.router.navigateByUrl('/inicioAdmin');
      return;
    }

    this.usuarioService.postLogin(user).subscribe((res: any) => {
      if(user.usr == res.correo && user.cntr == res.contrasenia){
        if(!res.able || !res.alta){
          this.router.navigateByUrl('/denied')
        }
      }
      if (user.usr == res.correo && user.cntr == res.contrasenia && res.able && res.alta) {
        usuarioLogueado.nombre = res.nombre;
        usuarioLogueado.apellido = res.apellido;
        usuarioLogueado.usuario = res.usuario;
        usuarioLogueado.numero = res.numero;
        usuarioLogueado.contrasenia = res.contrasenia;
        usuarioLogueado.correo = res.correo;
        console.log(usuarioLogueado)
        this.router.navigateByUrl('/inicio');
        this.mostrarMensaje = true;
      } else {
        this.mostrarMensajeError = true;
      }
      if (user.cntr != res.contrasenia) {
        this.contador++;
      }
      if (this.contador == 3) {
        this.deshabilitar();
        this.router.navigateByUrl('/denied')
      }
      this.correo.setValue('');
      this.contrasenia.setValue('');
    }, (err) => {
      this.mostrarMensajeError = true;
    })
  }

  desactivarMensaje() {
    this.mostrarMensaje = false
    this.mostrarMensajeError = false
  }

  getErrorMessage() {
    if (this.correo.hasError('required')) {
      return 'You must enter a value';
    }

    return this.correo.hasError('email') ? 'Not a valid email' : '';
  }

  olvideMiContr() {
    this.router.navigateByUrl('/OlvideMiContrasenia');
  }

  deshabilitar() {
    const user: Usuariolog = {
      usr: this.correo.value as string,
      cntr: this.contrasenia.value as string
    }

    this.usuarioService.postDeshabilitar(user).subscribe((res: any) => {

    }, (err) => {
      this.mostrarMensajeError = true;
    })
  }
}
