import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EditarCarpetasComponent } from './editar-carpetas.component';

describe('EditarCarpetasComponent', () => {
  let component: EditarCarpetasComponent;
  let fixture: ComponentFixture<EditarCarpetasComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EditarCarpetasComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(EditarCarpetasComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
