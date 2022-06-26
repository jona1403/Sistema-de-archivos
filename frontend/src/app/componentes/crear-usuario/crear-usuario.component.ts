import { Component, OnInit } from '@angular/core';
import { FormControl, Validators} from '@angular/forms';
import { Router } from '@angular/router';
import { Usuario } from 'src/app/models/usuario';
import { UsuarioService } from '../../services/usuario.service';

@Component({
  selector: 'app-crear-usuario',
  templateUrl: './crear-usuario.component.html',
  styleUrls: ['./crear-usuario.component.css']
})
export class CrearUsuarioComponent implements OnInit {
  hide = true;
  hide2 = true;
  Nombre = new FormControl('');
  Apellido = new FormControl('');
  Usuario = new FormControl('');
  Correo = new FormControl('', [Validators.required, Validators.email]);
  Telefono = new FormControl('');
  FechaNac = new FormControl('');
  contrasenia = new FormControl('');
  confirmContrasenia = new FormControl('');
  mostrarMensaje = false;
  mostrarMensajeError = false;

  constructor(private usuarioService: UsuarioService, public router: Router) { }

  ngOnInit(): void {
  }
  validacionCont(): boolean {
    const regex = new RegExp('(?=.*[A-Z])(?=.*[0-9])(?=.*[!"#$%&/()])([A-Za-z0-9!"#$%&/()]){8,20}');
    if (this.contrasenia.value == this.confirmContrasenia.value) {
      if (regex.test(this.contrasenia.value as string)) {
        return true;
      }
    }
    return false;
  }
  crearUsuario() {

    const tiempoTranscurrido = Date.now();
    const hoy = new Date(tiempoTranscurrido);

    const user: Usuario = {
      nombre: this.Nombre.value as string,
      apellido: this.Apellido.value as string,
      usuario: this.Usuario.value as string,
      correo: this.Correo.value as string,
      numero: Number(this.Telefono.value),
      contrasenia: this.contrasenia.value as string,
      fechaNac: this.FechaNac.value as string,
      fechaReg: hoy.toUTCString(),
      able: true,
      alta: false,
      type: 0
    }
    //Se valida la contrasenia
    if (!this.validacionCont()) {
      this.mostrarMensajeError = true;
      return;
    }
    this.usuarioService.postCreateCliente(user).subscribe((res: any) => {
      this.mostrarMensaje = true
      this.Nombre.setValue("")
      this.Apellido.setValue("")
      this.Usuario.setValue("")
      this.Correo.setValue("")
      this.Telefono.setValue("")
      this.FechaNac.setValue("")
      this.contrasenia.setValue("")
      this.confirmContrasenia.setValue("")
      this.router.navigateByUrl('/')
      console.log("Usuario Creado")
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
