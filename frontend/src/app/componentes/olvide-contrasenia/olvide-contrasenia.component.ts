import { Component, OnInit } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
import { Rec } from 'src/app/models/rec';
import { UsuarioService } from '../../services/usuario.service';

@Component({
  selector: 'app-olvide-contrasenia',
  templateUrl: './olvide-contrasenia.component.html',
  styleUrls: ['./olvide-contrasenia.component.css']
})
export class OlvideContraseniaComponent implements OnInit {

  usuario = new FormControl('')
  correo = new FormControl('')
  mostrarMensaje = false;
  mostrarMensajeError = false;

  constructor(private usuarioService: UsuarioService) { }

  ngOnInit(): void {
  }

  verificar(){
    const user: Rec={
      usr: this.usuario.value as string,
      correo: this.correo.value as string
    }

    this.usuarioService.postRecuperar(user).subscribe((res:any)=>{
      this.usuario.setValue("");
      this.correo.setValue("");
    },(err)=>{
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


}
