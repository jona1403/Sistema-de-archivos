import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DardeAltaComponent } from './darde-alta.component';

describe('DardeAltaComponent', () => {
  let component: DardeAltaComponent;
  let fixture: ComponentFixture<DardeAltaComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DardeAltaComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DardeAltaComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
