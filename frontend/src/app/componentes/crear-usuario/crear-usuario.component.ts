import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Usuario } from 'src/app/models/usuario';
import { UsuarioService } from '../../services/usuario.service';

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
  contrasenia = new FormControl('');
  confirmContrasenia = new FormControl('');
  mostrarMensaje = false;
  mostrarMensajeError = false;

  constructor(private usuarioService: UsuarioService) { }

  ngOnInit(): void {
  }
  validacionCont(): boolean {
    const regex = new RegExp('.+\\*.+');
    if (this.contrasenia.value == this.confirmContrasenia.value) {
      return true;

    }
    return false;
  }
  crearUsuario() {
    const user: Usuario = {
      nombre: this.Nombre.value,
      apellido: this.Apellido.value,
      usuario: this.Usuario.value,
      correo: this.Correo.value,
      numero: Number(this.Telefono.value),
      contrasenia: this.contrasenia.value,
      fechaNac: this.FechaNac.value,
      fechaReg: this.FechaCre.value,
      able: false,
      type: 0
    }

    if(!this.validacionCont()){
      return;
    }
    this.usuarioService.postCreateCliente(user).subscribe((res: any) => {
      this.mostrarMensaje = true
      this.Nombre.setValue("")
      this.Apellido.setValue("")
      this.Usuario.setValue("")
      this.Correo.setValue("")
      this.Telefono.setValue("")
      this.FechaCre.setValue("")
      this.FechaNac.setValue("")
      this.contrasenia.setValue("")
      this.confirmContrasenia.setValue("")
      console.log("Usuario Creado")
    }, (err) => {
      this.mostrarMensajeError = true
    })

  }

  desactivarMensaje() {
    this.mostrarMensaje = false
    this.mostrarMensajeError = false
  }

}
