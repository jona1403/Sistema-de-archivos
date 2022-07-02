import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { usuarioLogueado, carpetavar, archivovar } from 'src/app/apiURL/baseURL';
import { UsuarioService } from 'src/app/services/usuario.service';
import { FlatTreeControl } from '@angular/cdk/tree';
import { MatTreeFlatDataSource, MatTreeFlattener } from '@angular/material/tree';
import { CarpetaPropietario } from '../../models/carpeta-propietario';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
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
  typee: number;
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
      property: node.propietario,
      typee: node.tipo
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

  AgregarCarpeta(propietario: string) {
    const carpeta: CarpetaPropietario = {
      nombreCarpeta: this.nombreArch.value as string,
      nuevoPropietario: propietario,
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

  AgregarArchivo(propietario: string) {
    const carpeta: CarpetaPropietario = {
      nombreCarpeta: this.nombreArch.value as string,
      nuevoPropietario: propietario,
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

  Modificar(type: Number, nombre: string) {
    console.log(type)
    
    if (type == 0) {

      this.Carpetas();
      this.carpetasColaboracion();
      carpetavar.nombre = nombre;
      console.log(nombre)
      this.router.navigateByUrl('/inicio/EditarCarpetas')
    } else if (type == 1) {

      const carpeta: CarpetaPropietario = {
        nombreCarpeta: nombre,
        nuevoPropietario: "",
        nivel: 1
      }
      this.nombreArch.setValue("")
  
      console.log(carpeta);
      this.usuarioService.postMofifyFile(carpeta).subscribe((res: any) => {
        archivovar.texto = res.texto;
        console.log("AAAAAAAAAAAAAAAAAA")
        console.log(archivovar.texto)
      }, (err) => {
        console.log(err)
      })
      this.Carpetas();
      this.carpetasColaboracion();


      archivovar.nombre = nombre;
      this.router.navigateByUrl('/inicio/EditarArchivo')
    }

  }

  pdf(type: Number, name: string) {
    if (type == 1) {
      this.router.navigateByUrl('/inicio/SeePDF')
    }
  }

}
