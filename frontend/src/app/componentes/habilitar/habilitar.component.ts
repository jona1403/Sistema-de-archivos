import { Component, OnInit } from '@angular/core';
import { MatTableDataSource } from '@angular/material/table';
import { UsuarioService } from 'src/app/services/usuario.service';

export interface usr{
  nombre: string
  apellido: string
  usuario: string
  correo: string
  numero: number
  contrasenia: string
  fechaNac: string
  fechaReg: string
  able: boolean
  alta: boolean
  type: number
}


@Component({
  selector: 'app-habilitar',
  templateUrl: './habilitar.component.html',
  styleUrls: ['./habilitar.component.css']
})
export class HabilitarComponent implements OnInit {
  ELEMENT_DATA: usr[]= [];
  displayedColumns: string[] = ['usuario', 'nombre', 'apellido', 'correo', 'acciones'];
  dataSource !: MatTableDataSource<any>;
  constructor(private usuarioService: UsuarioService) { }

  ngOnInit(): void {
    this.UsuariosHabilitar();
  }

  UsuariosHabilitar() {
    this.usuarioService.postHabilitar("").subscribe((res: any) => {

      
      console.log(res)
      this.ELEMENT_DATA = res;
      this.dataSource  = new MatTableDataSource(this.ELEMENT_DATA);

    }, (err) => {
      console.log(err)
    })
  }

_Delete(index:number){
  console.log(index)
  this.usuarioService.postEliminar(this.ELEMENT_DATA[index]).subscribe((res:any)=>{
    console.log(res)
  })
  this.UsuariosHabilitar();
}
_Habilitar(index:number){
  console.log(index)
  this.usuarioService.postHabilitarusr(this.ELEMENT_DATA[index]).subscribe((res:any)=>{
    console.log(res)
  })
  this.UsuariosHabilitar();
}

}
