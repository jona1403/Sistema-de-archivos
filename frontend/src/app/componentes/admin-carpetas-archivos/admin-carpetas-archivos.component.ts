import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { MatTableDataSource } from '@angular/material/table';
import { UsuarioService } from 'src/app/services/usuario.service';
import { FlatTreeControl } from '@angular/cdk/tree';
import { MatTreeFlatDataSource, MatTreeFlattener } from '@angular/material/tree';
import { CarpetaPropietario } from 'src/app/models/carpeta-propietario';

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

/*interface FoodNode {
  name: string;
  children?: FoodNode[];
}

const TREE_DATA: FoodNode[] = [
  {
    name: 'Fruit',
    children: [{name: 'Apple'}, {name: 'Banana'}, {name: 'Fruit loops'}],
  },
  {
    name: 'Vegetables',
    children: [
      {
        name: 'Green',
        children: [{name: 'Broccoli'}, {name: 'Brussels sprouts'}],
      },
      {
        name: 'Orange',
        children: [{name: 'Pumpkins'}, {name: 'Carrots'}],
      },
    ],
  },
];*/

/** Flat node with expandable and level information */
interface ExampleFlatNode {
  expandable: boolean;
  name: string;
  level: number;
  property: string;
}






@Component({
  selector: 'app-admin-carpetas-archivos',
  templateUrl: './admin-carpetas-archivos.component.html',
  styleUrls: ['./admin-carpetas-archivos.component.css']
})
export class AdminCarpetasArchivosComponent implements OnInit {
  //ELEMENT_DATA = [];
  //displayedColumns: string[] = ['usuario', 'nombre', 'apellido', 'correo', 'acciones'];
  //dataSource !: MatTableDataSource<any>;
  //TREE_DATA = carpeta[];
  nombreNuevo = new FormControl();
  path = new FormControl('');







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











  constructor(private usuarioService: UsuarioService) {
    //this.dataSource.data = TREE_DATA;
  }



  hasChild = (_: number, node: ExampleFlatNode) => node.expandable;




  ngOnInit(): void {
    this.Syncronize();
  }


  open() {
    console.log(this.path.value)
  }


  Syncronize() {
    this.usuarioService.postSyncronize("").subscribe((res: any) => {


      console.log(res)
      this.dataSource.data = res;
      //this.dataSource = new MatTableDataSource(res);

    }, (err) => {
      console.log(err)
    })
  }

  addProperty(Carpeta: string, nivel: number) {


    const carpeta: CarpetaPropietario = {
      nombreCarpeta: Carpeta,
      nuevoPropietario: this.nombreNuevo.value as string,
      nivel: nivel
    }



    this.usuarioService.postAddProperty(carpeta).subscribe((res: any) => {
      console.log(res)
    }, (err) => {
      console.log(err)
    })
    this.Syncronize();
    this.nombreNuevo.setValue("")
  }

  addColaborador(Carpeta: string, nivel: number) {


    const carpeta: CarpetaPropietario = {
      nombreCarpeta: Carpeta,
      nuevoPropietario: this.nombreNuevo.value as string,
      nivel: nivel
    }



    this.usuarioService.postAddColaborador(carpeta).subscribe((res: any) => {
      console.log(res)
    }, (err) => {
      console.log(err)
    })
    this.Syncronize();
    this.nombreNuevo.setValue("")
  }


}
