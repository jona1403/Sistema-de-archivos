import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { usuarioLogueado } from 'src/app/apiURL/baseURL';
import { UsuarioService } from 'src/app/services/usuario.service';
import { FlatTreeControl } from '@angular/cdk/tree';
import { MatTreeFlatDataSource, MatTreeFlattener } from '@angular/material/tree';
import { CarpetaPropietario } from '../../models/carpeta-propietario';
import {MatDialog, MatDialogRef, MAT_DIALOG_DATA} from '@angular/material/dialog';
import { FormControl } from '@angular/forms';


export interface archivo {
  nombre: string,
  propietario: string,
  tipo: number,
  grupo: string[],
  texto: string
}
export interface carpeta {
  nombre: string,
  propietario: string,
  tipo: number,
  grupo: string[],
  contenido: Array<any>
}

var nombre = usuarioLogueado.usuario;

interface ExampleFlatNode {
  expandable: boolean;
  name: string;
  level: number;
  property: string;
}

@Component({
  selector: 'app-inicio',
  templateUrl: './inicio.component.html',
  styleUrls: ['./inicio.component.css']
})
export class InicioComponent implements OnInit {
  nombre = usuarioLogueado.usuario;
  nombreArch = new FormControl('');


  
  private _transformer = (node: carpeta, level: number) => {
    return {
      expandable: !!node.contenido && node.contenido.length > 0,
      name: node.nombre,
      level: level,
      property: node.propietario
    };
  };

  treeControl = new FlatTreeControl<ExampleFlatNode>(
    node => node.level,
    node => node.expandable,
  );

  treeFlattener = new MatTreeFlattener(
    this._transformer,
    node => node.level,
    node => node.expandable,
    node => node.contenido,
  );

  dataSource = new MatTreeFlatDataSource(this.treeControl, this.treeFlattener);
  dataSourceCol = new MatTreeFlatDataSource(this.treeControl, this.treeFlattener);

  constructor(public router: Router, private usuarioService: UsuarioService) { }


  hasChild = (_: number, node: ExampleFlatNode) => node.expandable;

  ngOnInit(): void {
    this.Carpetas();
    this.carpetasColaboracion();
  }
  Logout() {
    //logedin = false;
    this.router.navigateByUrl('/');
  }

  Carpetas() {
    this.usuarioService.postCarpetasUsuarios(usuarioLogueado).subscribe((res: any) => {


      console.log(res)
      this.dataSource.data = res;
      //this.dataSource = new MatTableDataSource(res);

    }, (err) => {
      console.log(err)
    })
  }

  carpetasColaboracion() {
    this.usuarioService.postCarpetasColaboracion(usuarioLogueado).subscribe((res: any) => {

      console.log("devielve")
      console.log(res)

      this.dataSourceCol.data = res;
      //this.dataSource = new MatTableDataSource(res);

    }, (err) => {
      console.log(err)
    })
  }

  EliminarCarpeta(Carpeta: string) {
    const carpeta: CarpetaPropietario = {
      nombreCarpeta: Carpeta,
      nuevoPropietario: "",
      nivel: 0
    }



    this.usuarioService.postDeleteFichero(carpeta).subscribe((res: any) => {
      console.log(res)
    }, (err) => {
      console.log(err)
    })
    this.Carpetas();
    this.carpetasColaboracion();
  }

  AgregarCarpeta() {
    const carpeta: CarpetaPropietario = {
      nombreCarpeta: this.nombreArch.value as string,
      nuevoPropietario: "",
      nivel: 0
    }
    this.nombreArch.setValue("")

    console.log(carpeta);
    this.usuarioService.postAddFichero(carpeta).subscribe((res: any) => {
      console.log(res)
    }, (err) => {
      console.log(err)
    })
    this.Carpetas();
    this.carpetasColaboracion();
  }

  AgregarArchivo() {
    const carpeta: CarpetaPropietario = {
      nombreCarpeta: this.nombreArch.value as string,
      nuevoPropietario: "",
      nivel: 1
    }
    this.nombreArch.setValue("")

    console.log(carpeta);
    this.usuarioService.postAddFichero(carpeta).subscribe((res: any) => {
      console.log(res)
    }, (err) => {
      console.log(err)
    })
    this.Carpetas();
    this.carpetasColaboracion();
  }

  ModificarCarpeta(Carpeta: string) {

  }

}
