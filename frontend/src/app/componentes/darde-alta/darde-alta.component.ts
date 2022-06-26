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
  selector: 'app-darde-alta',
  templateUrl: './darde-alta.component.html',
  styleUrls: ['./darde-alta.component.css']
})
export class DardeAltaComponent implements OnInit {
  ELEMENT_DATA: usr[]= [];
  displayedColumns: string[] = ['usuario', 'nombre', 'apellido', 'correo', 'acciones'];
  dataSource !: MatTableDataSource<any>;
  constructor(private usuarioService: UsuarioService){}

  ngOnInit(): void {
    this.UsuariosDarAlta();
  }


//Aqui se obtienen los datos para la tabla a partir de un servicio
  UsuariosDarAlta() {
    this.usuarioService.postDarAlta("").subscribe((res: any) => {

      
      console.log(res)
      this.ELEMENT_DATA = res;
      this.dataSource  = new MatTableDataSource(this.ELEMENT_DATA);

    }, (err) => {
      console.log(err)
    })
  }


  //Aqui se dan de alta y se envia solo el usuario a dar de alta y se elimina de la tabla que tenemos
  DarAlta(index: number){
    console.log(index)
    this.usuarioService.postEnviarAlta(this.ELEMENT_DATA[index]).subscribe((res:any)=>{
      console.log(res)
    })
    this.UsuariosDarAlta();
  }
}
